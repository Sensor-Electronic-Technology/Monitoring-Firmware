#include "MonitoringController.h"

namespace MonitoringComponents {

	void MonitoringController::Build() {
		ConfigurationReader reader;
		reader.Init();
		auto discreteConfig = reader.DeserializeDigitalConfig();
		//discreteInputs = discreteConfig.size();
		auto analogConfig = reader.DeserializeAnalogConfig();
		//inputRegisters = analogConfig.size();
		auto outputConfig = reader.DeserializeOutputConfig();
		//inputs = outputConfig.size();
		auto actionConfig = reader.DeserializeActions();

		auto netConfig = reader.DeserializeNetConfiguration();
		ModbusService::Initialize(netConfig);

		for (auto output : outputConfig) {
			DiscreteOutputChannel* channel = new DiscreteOutputChannel(output);
			this->outputChannels.push_back(channel);
		}

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
					//std::cout << "Creating ActionOutput 1: " << (*outputChannel)->Address().channel << "," << (*outputChannel)->Address().slot << std::endl;
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel1, actionConfig[i].offLevel1);
					action->SetOutput(output, 1);
				}
			}

			if (actionConfig[i].addr2) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr2 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					//std::cout << "Creating ActionOutput 2: " << (*outputChannel)->Address().channel << "," << (*outputChannel)->Address().slot << std::endl;
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel2, actionConfig[i].offLevel2);
					action->SetOutput(output, 2);
				}
			}

			if (actionConfig[i].addr3) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr3 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					//std::cout << "Creating ActionOutput 3: " << (*outputChannel)->Address().channel << "," << (*outputChannel)->Address().slot << std::endl;
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel3, actionConfig[i].offLevel3);
					action->SetOutput(output, 3);
				}
			}

			this->actions.push_back(action);
		}

		for (auto ch : discreteConfig) {
			DiscreteInputChannel* channel = new DiscreteInputChannel(ch);
			this->discreteInputs.push_back(channel);
			RegisterChild(channel);
			channel->OnStateChange(this->_on_channel_cbk);
		}

		for (auto ch : analogConfig) {
			AnalogInputChannel* channel = new AnalogInputChannel(ch);
			this->analogInputs.push_back(channel);
			RegisterChild(channel);
			channel->OnStateChange(this->_on_channel_cbk);
		}
	}

	void MonitoringController::OnChannelCallback(ChannelCallback cbk) {
		this->_on_channel_cbk = cbk;
	}

	void MonitoringController::Setup() {
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Alarm, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Warning, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::SoftWarn, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Maintenance, false));
		this->systemActionLatches.insert(std::pair<ActionType, bool>(ActionType::Okay, false));

		for (auto actionLatches : systemActionLatches) {
			cout << "ActionType: " << (int)actionLatches.first << " State: " << actionLatches.second << endl;
		}

		this->OnChannelCallback([&](ChannelMessage message) {
				ProcessChannelMessage(message);
			});
		
		this->Build();

		this->printTimer.onInterval([&]() {
			cout << "Latches: "<< endl;
			for (auto actionLatches : systemActionLatches) {
				cout << "ActionType: " << (int)actionLatches.first << " State: " << actionLatches.second << endl;
			}

			switch(controllerState) {
				case ControllerState::Alarming: {
					cout << "ControllerState: " << "Alarming" << endl;
					break;
				}

				case ControllerState::Maintenance:{
					cout << "ControllerState: " << "Maintenance" << endl;
					break;
				}

				case ControllerState::Warning:{
					cout << "ControllerState: " << "Warning" << endl;
					break;
				}

				case ControllerState::Okay:{
					cout << "ControllerState: " << "Okay" << endl;
					break;
				}
			}

			cout << "Action Registrations: " << endl;
			for(auto registration : actionTracking) {
				cout <<"Id: "<<registration.first <<" Instances: "<< registration.second->size() << endl;
			}
		}, 500);
		RegisterChild(this->printTimer);
		RegisterChild(this->checkStateTimer);
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

		for (auto ainput : analogInputs) {
			ainput->Initialize();
		}

		ProcessStateChanges();
		//this->checkStateTimer.onInterval([&]() {
		//	ProcessStateChanges();
		//}, 100);
	}

	void MonitoringController::ProcessChannelMessage(ChannelMessage message) {	
		auto action = find_if(actions.begin(), actions.end(), [&](Action* act) {
			return message.actionId == act->Id();
		});

		if (action != actions.end()) {
			int id = (*action)->Id();
			Registrations* registrations = actionTracking[id];
			bool isNew = false;
			if (registrations == nullptr) {
				registrations = new Registrations;
				actionTracking[id] = registrations;
				isNew = true;
			}

			if (message.channelAction == ChannelAction::Trigger) {
				if (isNew) {
					registrations->push_back(message.channel);
					if (message.type == ActionType::Custom) {
						(*action)->Invoke();
					} else {
						this->systemActionLatches[message.type] = true;
						this->ProcessStateChanges();
					}
				} else {
					auto channel = find_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
						return address == message.channel;
					});
					if (channel == registrations->end()) {
						registrations->push_back(message.channel);
						if (message.type == ActionType::Custom) {
							(*action)->Invoke();
						} else {
							this->systemActionLatches[message.type] = true;
							this->ProcessStateChanges();
						}
					}
				}
				//this->systemActionLatches[message.type] = true;
			} else if (message.channelAction == ChannelAction::Clear) {
				auto channel = registrations->erase(remove_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
						return address == message.channel;
				}), registrations->end());
				if (registrations->empty()) {	
					if (message.type == ActionType::Custom) {
						(*action)->Clear();
					} else {
						this->systemActionLatches[message.type] = false;
						this->ProcessStateChanges();
					}
				}
			}
		}
	}

	void MonitoringController::InvokeSystemAction(ActionType actionType) {
		if (actionType != ActionType::Custom) {
			int index = systemActMap[actionType];
			Action* action = actions[index];
			action->Invoke();
		}
	}

	void MonitoringController::ProcessStateChanges() {
		if (systemActionLatches[ActionType::Maintenance]) {
			if (this->controllerState != ControllerState::Maintenance) {
				this->controllerState = ControllerState::Maintenance;
				this->InvokeSystemAction(ActionType::Maintenance);
			}
		} else if (systemActionLatches[ActionType::Alarm]) {
			if (this->controllerState != ControllerState::Alarming) {
				this->controllerState = ControllerState::Alarming;
				this->InvokeSystemAction(ActionType::Alarm);
			}
		} else if (systemActionLatches[ActionType::Warning]) {
			if (this->controllerState != ControllerState::Warning) {
				this->controllerState = ControllerState::Warning;
				this->InvokeSystemAction(ActionType::Warning);
			}
		} else {
			if (this->controllerState != ControllerState::Okay) {
				this->controllerState = ControllerState::Okay;
				this->InvokeSystemAction(ActionType::Okay);
			}
		}
	}

	void MonitoringController::Run() {
		this->loop();
	}

	void MonitoringController::privateLoop() {

	}
};