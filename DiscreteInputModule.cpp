#include "DiscreteInputModule.h"

namespace MonitoringComponents {

	int MonitoringComponents::DiscreteInputModule::Slot() {
		return this->slot;
	}


	void DiscreteInputModule::CreateChannels(std::vector<DigitalInConfiguration> channelConfigurations) {
		for (int i = 0; i < channelConfigurations.size(); i++) {
			auto channel = new DiscreteInputChannel(DigitalInConfiguration(channelConfigurations[i]));
			this->channels.push_back(channel);
			RegisterChild(channel);
			channel->OnTrigger([](Ref<DiscreteInputChannel> channel) {
				Serial.println("Channel Triggered!");
				channel->Print();
			});
		}
		this->isInitialized.channelsSet = true;
		Serial.println("Channels Set!");
	}

	void DiscreteInputModule::SetConfig(ModuleConfiguration config) {
		this->configuration = config;
		this->isInitialized.configSet = true;
	}


	void DiscreteInputModule::Print() {
		for (auto channel : this->channels)
			channel->Print();
	}

	//void DiscreteInputModule::OnChannelTrigger(DiscreteInputCallBack cbk) {
	//	this->_on_Channel_Trigger = cbk;
	//}

	void DiscreteInputModule::privateLoop() {
		std::cout << "Module private Loop" << std::endl;
	}
};


