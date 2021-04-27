#include "MonitoringController.h"
namespace MonitoringComponents {

	void MonitoringController::BuildChannels() {
		ConfigurationReader reader;
		reader.Init();
		auto discreteConfig = reader.DeserializeDigitalConfig();
		auto analogConfig = reader.DeserializeAnalogConfig();
		auto outputConfig = reader.DeserializeOutputConfig();

		auto cbk = [&](ChannelMessage msg) {
			//think i will need to track state
			if (msg.action == ChannelAction::Trigger) {
				//add to alert map
				
			} else {
				//remove from alert map
			}
		};

		for (auto ch : discreteConfig) {
			DiscreteInputChannel* channel = new DiscreteInputChannel(ch);
			this->discreteInputs.push_back(channel);
			//channel->OnTrigger()
		}
	}
};