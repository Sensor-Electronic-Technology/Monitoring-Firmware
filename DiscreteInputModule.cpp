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
			channel->OnTrigger(this->_on_channel_trigger);
			channel->OnClear(this->_on_channel_clear);
		}

		this->isInitialized.channelsSet = true;
	}

	void DiscreteInputModule::Initialize() {
		for (int i = 0; i < channels.size(); i++) {
			channels[i]->Initialize();
		}

		timer.onInterval([&]() {
			cout << F("Triggered Channels: ");
			for (int i = 0; i < triggeredChannels.size(); i++) {
				cout<< triggeredChannels[i]<<' ';
			}
			cout << endl; 
		}, 500);
		RegisterChild(timer);
	}

	void DiscreteInputModule::SetConfig(ModuleConfiguration config) {
		this->configuration = config;
		this->isInitialized.configSet = true;
	}

	void DiscreteInputModule::OnChannelTrigger(DiscreteInputCallback cbk) {
		this->_on_channel_trigger = cbk;
	}

	void DiscreteInputModule::OnChannelClear(DiscreteInputCallback cbk) {
		this->_on_channel_clear = cbk;
	}

	void DiscreteInputModule::privateLoop() {

	}
};


