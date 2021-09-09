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

		for(auto action : this->actions) {
			this->actionTracking.insert(std::pair<int, Registrations*>(action->Id(), new Registrations));
		}

		this->OnChannelCallback([&](ChannelMessage message) {
				ProcessChannelMessage(message);
			});
		
		this->Build();

		this->printTimer.onInterval([&]() {
			String buffer;
			MonitoringLogger::LogInfo(F("Latches"));
			for (auto actionLatches : systemActionLatches) {
				MonitoringLogger::LogInfo(F("ActionType: %u State: %u"), (int)actionLatches.first, actionLatches.second);
				//cout << "ActionType: " << (int)actionLatches.first << " State: " << actionLatches.second << endl;
			}

			switch(controllerState) {
				case ControllerState::Alarming: {
					//cout << "ControllerState: " << "Alarming" << endl;
					MonitoringLogger::LogInfo(F("ControllerState: Alarming"));
					break;
				}

				case ControllerState::Maintenance:{
					//cout << "ControllerState: " << "Maintenance" << endl;
					MonitoringLogger::LogInfo(F("ControllerState: Maintenance"));
					break;
				}

				case ControllerState::Warning:{
					//cout << "ControllerState: " << "Warning" << endl;
					MonitoringLogger::LogInfo(F("ControllerState: Warning"));
					break;
				}

				case ControllerState::Okay:{
					//cout << "ControllerState: " << "Okay" << endl;
					MonitoringLogger::LogInfo(F("ControllerState: Okay"));
					break;
				}
			}

			//cout << "Action Registrations: " << endl;
			MonitoringLogger::LogInfo(F("Action Registrations: "));
			for(auto registration : actionTracking) {
				//cout <<"Id: "<<registration.first <<" Instances: "<< registration.second->size() << endl;
				MonitoringLogger::LogInfo(F("Id: %u Instances: %u"), registration.first, registration.second);
			}
		}, 1000);

		MonitoringLogger::LogInfo(F("Latches"));
		for(auto actionLatches : systemActionLatches) {
			MonitoringLogger::LogInfo(F("ActionType: %u State: %u"), (int)actionLatches.first, actionLatches.second);
		}

		MonitoringLogger::LogInfo(F("ActionTracking: "));
		for(auto tracking : this->actionTracking) {
			MonitoringLogger::LogInfo(F("ActionId: %d  Empty?  %t"),tracking.first,tracking.second->empty());
		}
		
		
		RegisterChild(this->printTimer);
		//RegisterChild(this->checkStateTimer);
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
	}

	void MonitoringController::ProcessChannelMessage(ChannelMessage message) {

		auto action = find_if(actions.begin(), actions.end(), [&](Action* act) {
			return message.actionId == act->Id();
		});

		if(action == actions.end()) {
			MonitoringLogger::LogError(F("Invalid ActionId: %d  From Channel(Slot,Channel): (%d,%d)"),message.actionId,message.channel.slot,message.channel.channel);
			return;
		}

		int id = (*action)->Id();
		Registrations* registrations = actionTracking[id];
		bool isNew = false;
		//if (registrations == nullptr) {
		//	registrations = new Registrations;
		//	actionTracking[id] = registrations;
		//	isNew = true;
		//}

		if(registrations == nullptr) {
			MonitoringLogger::LogError(F("ActionTracking does not contain registrations for ActionId: %d"),id);
			return;
		}

		auto channel = find_if(registrations->end(), registrations->end(), [&](ChannelAddress address) { 
			return address == message.channel;
		});

		if(channel==registrations->end()) {
			switch(message.channelAction) {
				case ChannelAction::Trigger:{
					registrations->push_back(message.channel);
					
					break;
				}
				case ChannelAction::Clear:{
					auto channel = registrations->erase(remove_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
							return address == message.channel;
						}), registrations->end());
					break;
				}
			}
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


	//if(message.channelAction == ChannelAction::Trigger) {
	//	if(isNew) {
	//		registrations->push_back(message.channel);
	//		if(message.type == ActionType::Custom) {
	//			(*action)->Invoke();
	//		} else {
	//			this->systemActionLatches[message.type] = true;
	//			this->ProcessStateChanges();
	//		}
	//	} else {
	//		auto channel = find_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
	//			return address == message.channel;
	//			});
	//		if(channel == registrations->end()) {
	//			registrations->push_back(message.channel);
	//			if(message.type == ActionType::Custom) {
	//				(*action)->Invoke();
	//			} else {
	//				this->systemActionLatches[message.type] = true;
	//				this->ProcessStateChanges();
	//			}
	//		}
	//	}
	//} else if(message.channelAction == ChannelAction::Clear) {
	//	auto channel = registrations->erase(remove_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
	//		return address == message.channel;
	//		}), registrations->end());
	//	if(registrations->empty()) {
	//		if(message.type == ActionType::Custom) {
	//			(*action)->Clear();
	//		} else {
	//			this->systemActionLatches[message.type] = false;
	//			this->ProcessStateChanges();
	//		}
	//	}
	//}
};