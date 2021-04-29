#include "MonitoringController.h"
namespace MonitoringComponents {

	void MonitoringController::BuildChannels() {
		ConfigurationReader reader;
		reader.Init();
		auto discreteConfig = reader.DeserializeDigitalConfig();
		auto analogConfig = reader.DeserializeAnalogConfig();
		auto outputConfig = reader.DeserializeOutputConfig();

		auto cbk = [&](ChannelMessage message) {
			if (message.action == ChannelAction::Trigger) {
				auto found = std::find_if(triggeredChannels.begin(), triggeredChannels.end(), [&](DistinctChannel ch) {
						return ch == message.channel;
					});
				if (found == triggeredChannels.end()) {
					triggeredChannels.push_back(message.channel);
				}
			} else if (message.action == ChannelAction::Clear) {
				triggeredChannels.erase(std::remove_if(triggeredChannels.begin(), triggeredChannels.end(), [&](DistinctChannel ch) {
						return ch == message.channel;
					}), triggeredChannels.end());
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
			//channel->OnStateChange(cbk);
		}
	}
};