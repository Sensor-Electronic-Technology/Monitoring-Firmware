#include "MonitoringController.h"


namespace MonitoringComponents {

	void MonitoringController::Setup() {

		MonitoringLogger::Start(&Serial, LogLevel::Info);

		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Alarm, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Warning, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::SoftWarn, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Maintenance, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Okay, false));

		this->OnChannelCallback([&](ChannelMessage message) {
				ProcessChannelMessage(message);
			});

		this->Build();

		for(auto action : this->actions) {
			this->tracking.insert(std::pair<int, int*>(action->Id(), new int(0)));
		}

		this->printTimer.onInterval([&]() {
			Print();
		}, 1000);

		MonitoringLogger::LogInfo(F("Latches"));
		for(auto actionLatches : systemActionLatches) {
			MonitoringLogger::LogInfo(F("ActionType: %u State: %u"), (int)actionLatches.first, actionLatches.second);
		}

		MonitoringLogger::LogInfo(F("Action Registrations: "));
		for(auto registration : tracking) {
			MonitoringLogger::LogInfo(F("Id: %u Instances: %u"), registration.first, (*registration.second));
		}

		RegisterChild(this->printTimer);
		//RegisterChild(this->checkStateTimer);
	}
	
	void MonitoringController::Build() {
		ConfigurationReader reader;
		reader.Init();
		auto discreteConfig = reader.DeserializeDigitalConfig();
		auto analogConfig = reader.DeserializeAnalogConfig();
		auto outputConfig = reader.DeserializeOutputConfig();
		auto actionConfig = reader.DeserializeActions();
		auto virtualConfig = reader.DeserializeVirtualConfig();
		this->modules = reader.DeserializeModuleConfig();
		auto netConfig = reader.DeserializeNetConfiguration();

		MonitoringLogger::EnableFileLogger();

		ModbusService::Initialize(netConfig);

		//Outputs needed for building actions
		for (auto output : outputConfig) {
			DiscreteOutputChannel* channel = new DiscreteOutputChannel(output);
			this->outputChannels.push_back(channel);
		}

		MonitoringLogger::LogInfo(F("Creating Actions"));

		for (int i = 0; i < actionConfig.size();i++) {

			Action* action=new Action(actionConfig[i]);
			
			if (actionConfig[i].actionType != ActionType::Custom) {
				this->systemActMap[actionConfig[i].actionType] = i;
			}

			if (actionConfig[i].addr1) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr1 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel1, actionConfig[i].offLevel1);
					action->SetOutput(output, 1);
				}
			}

			if (actionConfig[i].addr2) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr2 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel2, actionConfig[i].offLevel2);
					action->SetOutput(output, 2);
				}
			}

			if (actionConfig[i].addr3) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr3 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel3, actionConfig[i].offLevel3);
					action->SetOutput(output, 3);
				}
			}
			this->actions.push_back(action);
		}


		MonitoringLogger::LogInfo(F("Creating Discrete Channels"));
		for (auto ch : discreteConfig) {
			DiscreteInputChannel* channel = new DiscreteInputChannel(ch);
			this->discreteInputs.push_back(channel);
			RegisterChild(channel);
			channel->OnStateChange(this->_on_channel_cbk);
		}

		for (auto ch : virtualConfig) {
			DiscreteVirtualChannel* channel = new DiscreteVirtualChannel(ch);
			this->virtualInputs.push_back(channel);
			RegisterChild(channel);
			channel->OnStateChange(this->_on_channel_cbk);
		}

		MonitoringLogger::LogInfo(F("Creating Analog Channels"));
		for (auto ch : analogConfig) {
			AnalogInputChannel* channel = new AnalogInputChannel(ch);
			this->analogInputs.push_back(channel);
			RegisterChild(channel);
			channel->OnStateChange(this->_on_channel_cbk);
		}
	}

	void MonitoringController::Initialize() {

		for (auto output : outputChannels) {
			output->SetOutput(State::Low);
		}

		for (auto action : actions) {
			action->Clear();
		}

		for (auto dinput : discreteInputs) {
			dinput->Initialize();
		}

		for (auto vInput : virtualInputs) {
			vInput->Initialize();
		}

		for (auto ainput : analogInputs) {
			ainput->Initialize();
		}

		ProcessStateChanges();
	}

	void MonitoringController::OnChannelCallback(ChannelCallback cbk) {
		this->_on_channel_cbk = cbk;
	}

	void MonitoringController::ProcessChannelMessage(ChannelMessage message) {

		auto action = find_if(actions.begin(), actions.end(), [&](Action* act) {
			return message.actionId == act->Id();
		});

		if(action == actions.end()) {
			MonitoringLogger::LogError(F("Invalid ActionId: %d  From Channel(Slot,Channel): (%d,%d)"),
				message.actionId,message.channel.slot,message.channel.channel);
			return;
		}

		int id = (*action)->Id();
		int* actionCount = tracking[id];

		switch(message.channelAction) {
			case ChannelAction::Trigger: {
				if(message.type == ActionType::Custom) {
					(*action)->Invoke();
				} else {
					(*actionCount) += 1;
					this->systemActionLatches[message.type] = true;
					this->ProcessStateChanges();
				}
				break;
			}
			case ChannelAction::Clear:{
				if(message.type == ActionType::Custom) {
					(*action)->Clear();
				} else {
					(*actionCount) -= 1;
					if((*actionCount)==0) {
						this->systemActionLatches[message.type] = false;
						(*action)->Clear();
						this->ProcessStateChanges();
					} else if((*actionCount) < 0) {
						(*actionCount) = 0;
						MonitoringLogger::LogError(F("Action Id: %d is negative. Count: %d"),id,(*actionCount));
					}
				}
				break;
			}
		}
	}

	void MonitoringController::InvokeSystemAction(ActionType actionType) {
		auto action = find_if(this->actions.begin(), this->actions.end(), [&](Action* act) {
			return actionType == act->GetActionType();
		});

		if (actionType != ActionType::Custom) {
			int index = systemActMap[actionType];
			Action* action = actions[index];
			action->Invoke();
		}
	}

	void MonitoringController::Print(){
		String buffer;
		MonitoringLogger::LogInfo(F("Latches"));
		for (auto actionLatches : systemActionLatches) {
			MonitoringLogger::LogInfo(F("ActionType: %u State: %u"), (int)actionLatches.first, actionLatches.second);
		}

		switch (controllerState) {
			case ControllerState::Alarming:{
				MonitoringLogger::LogInfo(F("ControllerState: Alarming"));
				break;
			}

			case ControllerState::Maintenance:{
				MonitoringLogger::LogInfo(F("ControllerState: Maintenance"));
				break;
			}

			case ControllerState::Warning:{
				MonitoringLogger::LogInfo(F("ControllerState: Warning"));
				break;
			}

			case ControllerState::Okay:{
				MonitoringLogger::LogInfo(F("ControllerState: Okay"));
				break;
			}
		}

		MonitoringLogger::LogInfo(F("Action Registrations: "));
		for (auto registration : tracking) {
			MonitoringLogger::LogInfo(F("Id: %u Instances: %u"), registration.first, (*registration.second));
		}
	}

	bool MonitoringController::CheckController(){
		int moduleCount=this->modules.size();
		int baseErrors=P1.rollCall(&this->modules[0], moduleCount);
		return baseErrors==0;
	}

	void MonitoringController::ProcessStateChanges() {
		if (systemActionLatches[ActionType::Maintenance]) {
			if (this->controllerState != ControllerState::Maintenance) {
				this->controllerState = ControllerState::Maintenance;
				this->InvokeSystemAction(ActionType::Maintenance);
				MonitoringLogger::LogInfo("State Changed to Maintenance");
			}
		} else if (systemActionLatches[ActionType::Alarm]) {
			if (this->controllerState != ControllerState::Alarming) {
				this->controllerState = ControllerState::Alarming;
				this->InvokeSystemAction(ActionType::Alarm);
				MonitoringLogger::LogInfo("State Changed to Alarm");
			}
		} else if (systemActionLatches[ActionType::Warning]) {
			if (this->controllerState != ControllerState::Warning) {
				this->controllerState = ControllerState::Warning;
				this->InvokeSystemAction(ActionType::Warning);
				MonitoringLogger::LogInfo("State Changed to Warning");
			}
		} else {
			if (this->controllerState != ControllerState::Okay) {
				this->controllerState = ControllerState::Okay;
				this->InvokeSystemAction(ActionType::Okay);
				MonitoringLogger::LogInfo("State changed to Okay");
			}
		}
	}

	void MonitoringController::Run() {
		this->loop();
	}

	void MonitoringController::privateLoop() {	}
};