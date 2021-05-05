#include "MonitoringController.h"

namespace MonitoringComponents {

	void MonitoringController::BuildChannels() {
		ConfigurationReader reader;
		reader.Init();
		auto discreteConfig = reader.DeserializeDigitalConfig();
		//auto analogConfig = reader.DeserializeAnalogConfig();
		auto outputConfig = reader.DeserializeOutputConfig();
		auto actionConfig = reader.DeserializeActions();

		for (auto output : outputConfig) {
			DiscreteOutputChannel* channel = new DiscreteOutputChannel(output);
			this->outputChannels.push_back(channel);
		}
		for (int i = 0; i < actionConfig.size();i++) {
			Action* action=new Action(actionConfig[i]);
			//this->systemIndexes=
			if (actionConfig[i].addr1) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr1 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					std::cout << "Creating ActionOutput 1: " << (*outputChannel)->Address().channel << "," << (*outputChannel)->Address().slot << std::endl;
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel1, actionConfig[i].offLevel1);
					action->SetOutput(output, 1);
				}
			}

			if (actionConfig[i].addr2) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr2 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					std::cout << "Creating ActionOutput 2: " << (*outputChannel)->Address().channel << "," << (*outputChannel)->Address().slot << std::endl;
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel2, actionConfig[i].offLevel2);
					action->SetOutput(output, 2);
				}
			}

			if (actionConfig[i].addr3) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return actionConfig[i].addr3 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					std::cout << "Creating ActionOutput 3: " << (*outputChannel)->Address().channel << "," << (*outputChannel)->Address().slot << std::endl;
					ActionOutput* output = new ActionOutput((*outputChannel), actionConfig[i].onLevel3, actionConfig[i].offLevel3);
					action->SetOutput(output, 3);
				}
			}
			this->actions.push_back(action);
		}

		this->OnChannelCallback([&](ChannelMessage message) {
			HandleAction(message);
		});

		for (auto ch : discreteConfig) {
			DiscreteInputChannel* channel = new DiscreteInputChannel(ch);
			std::cout << "Input Channel: {" << ch.address.channel << "," << ch.address.slot << "} ";
			this->discreteInputs.push_back(channel);
			RegisterChild(channel);
			channel->OnStateChange(this->_on_channel_cbk);
		}
		std::cout << std::endl;

		//for (auto config : analogConfig) {
		//	AnalogInputChannel* channel = new AnalogInputChannel(config);
		//	this->analogInputs.push_back(channel);
		//	RegisterChild(channel);
		//	channel->OnChannelTrigger(cbk);
		//}
	}

	void MonitoringController::OnChannelCallback(ChannelCallback cbk) {
		this->_on_channel_cbk = cbk;
	}

	void MonitoringController::privateLoop(){

	}

	void MonitoringController::Setup() {
		this->BuildChannels();
		this->printTimer.onInterval([&]() {
			std::cout << "Triggered Channels by Action" << std::endl;
			for (auto key : actionTracking) {
				std::cout << "Action[" << key.first << "]" << " Channels Tracked: ";
				for (auto channel : (*key.second)) {
					std::cout << "{"<<channel.channel << "," << channel.slot << "} ";
				}
				std::cout << std::endl;
			}
			//std::cout << "Free Ram: " << FreeRam() << std::endl;
		}, 500);
		RegisterChild(this->printTimer);
	}

	void MonitoringController::Initialize() {
		for (auto dinput : discreteInputs) {
			dinput->Initialize();
		}

		//for (auto ainput : analogInputs) {
		//	ainput->Initialize();
		//}

		for (auto output : outputChannels) {
			output->SetOutput(State::Low);
		}

		//for (auto action : actions) {
		//	action->Clear();
		//}
	}

	void MonitoringController::SetState(ActionType actionType) {
		switch (actionType) {
			case ActionType::Alarm:{
				if (controllerState != ControllerState::Maintenance) {
					this->controllerState = ControllerState::Alarming;
				}
				break;
			}
			case ActionType::Maintenance: {
				this->controllerState = ControllerState::Maintenance;
				break;
			}
			case ActionType::Warning: {
				if (controllerState != ControllerState::Alarming && controllerState!=ControllerState::Maintenance) {
					this->controllerState = ControllerState::Warning;
				}
				break;
			}
			default: {
				break;
			}
		}
	}

	void MonitoringController::HandleAction(ChannelMessage message) {
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
					(*action)->Invoke();
				} else {
					if (message.type!=ActionType::Custom || message.type!=ActionType::Okay) {
						//Alarm,Warning,SoftWarn,or Maintence

					}

					auto channel = find_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
						return address == message.channel;
					});
					if (channel == registrations->end()) {
						registrations->push_back(message.channel);
						(*action)->Invoke();
					}//else channel already triggered.  do not trigger again
				}
			} else if (message.channelAction == ChannelAction::Clear) {
				auto channel = registrations->erase(remove_if(registrations->begin(), registrations->end(), [&](ChannelAddress address) {
						return address == message.channel;
				}), registrations->end());

				if (registrations->empty()) {
					(*action)->Clear();			
				}
			}
		}
	}


	Action* MonitoringController::FindActionByType(ActionType actionType) {
		auto action = std::find_if(actions.begin(), actions.end(), [&](Action* act) {
				return act->ActionType() == actionType;
			});
		return *action;
	}

	bool MonitoringController::ActionCleared(ActionType actionType) {
		auto action = std::find_if(actions.begin(), actions.end(), [&](Action* act) {
			return act->ActionType() == actionType;
			});
		return *action;
	}
	
	bool MonitoringController::AlarmsCleared() {

		//return this->
	}

	


	void MonitoringController::Run() {
		this->loop();
	}



};