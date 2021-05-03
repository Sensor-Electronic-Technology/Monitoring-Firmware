#include "MonitoringController.h"
namespace MonitoringComponents {

	void MonitoringController::BuildChannels() {
		ConfigurationReader reader;
		reader.Init();
		auto discreteConfig = reader.DeserializeDigitalConfig();
		auto analogConfig = reader.DeserializeAnalogConfig();
		auto outputConfig = reader.DeserializeOutputConfig();
		auto actionConfig = reader.DeserializeActions();

		for (auto output : outputConfig) {
			DiscreteOutputChannel* channel = new DiscreteOutputChannel(output);
			this->outputChannels.push_back(channel);
		}


		for (auto act : actionConfig) {

			Action* action=new Action(act);


			if (act.addr1) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return act.addr1 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					ActionOutput* output = new ActionOutput(*outputChannel, act.outputlevel1);
					action->SetOutput(output, 1);
				}
			}

			if (act.addr2) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return act.addr2 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					ActionOutput* output = new ActionOutput(*outputChannel, act.outputlevel2);
					action->SetOutput(output, 2);
				}

			}

			if (act.addr3) {
				auto outputChannel=std::find_if(outputChannels.begin(), outputChannels.end(), [&](DiscreteOutputChannel* output) {
					return act.addr3 == output->Address();
				});
				if (outputChannel != outputChannels.end()) {
					auto value = (*outputChannel);
					ActionOutput* output = new ActionOutput(value, act.outputlevel3);
					action->SetOutput(output, 3);
				}
			}

			this->actions.push_back(action);

		}


		auto cbk = [&](ChannelMessage message) {
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
					}),registrations->end());
					if (registrations->empty()) {
						(*action)->Clear();
					}
					//(*action)->Clear();
				}
			}
		};


		for (auto ch : discreteConfig) {
			DiscreteInputChannel* channel = new DiscreteInputChannel(ch);
			this->discreteInputs.push_back(channel);
			channel->OnStateChange(cbk);
		}

		for (auto config : analogConfig) {
			AnalogInputChannel* channel = new AnalogInputChannel(config);
			this->analogInputs.push_back(channel);
			channel->OnChannelTrigger(cbk);
		}
	}
};