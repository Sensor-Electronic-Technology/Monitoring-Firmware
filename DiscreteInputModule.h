#pragma once
#include <ArduinoSTL.h>
#include "P1AM.h"
#include "IModule.h"
#include "DiscreteInputChannel.h"
#include "Timer.h"

namespace MonitoringComponents {
	class DiscreteInputModule :public IModule {
	public:
		DiscreteInputModule(ModuleConfiguration configuration, Ref<MonitoringComponent> parent = nullptr)
			:IModule(configuration,parent), _on_channel_trigger([](Ref<DiscreteInputChannel>) {}), _on_channel_clear([](Ref<DiscreteInputChannel>) {}) {
			this->BuildCallbacks();
		}

		DiscreteInputModule(Ref<MonitoringComponent> parent = nullptr) 
			:IModule(parent), _on_channel_trigger([](Ref<DiscreteInputChannel>) {}), _on_channel_clear([](Ref<DiscreteInputChannel>) {}) {
			this->BuildCallbacks();
		}

		int Slot();
		void CreateChannels(std::vector<DigitalInConfiguration> channelConfigurations);
		void SetConfig(ModuleConfiguration config);
		void Initialize();
		void OnChannelTrigger(DiscreteInputCallback cbk);
		void OnChannelClear(DiscreteInputCallback cbk);
		
	private:
		std::vector<DiscreteInputChannel*> channels;
		std::vector<int> triggeredChannels;
		Initialized isInitialized;
		DiscreteInputCallback _on_channel_trigger;
		DiscreteInputCallback _on_channel_clear;
		Timer timer;

		void BuildCallbacks() {
			OnChannelTrigger([&](Ref<DiscreteInputChannel> channel) {
				cout << "Channel " << channel->Channel() << " Triggered" << endl;
				auto found = std::find(triggeredChannels.begin(),triggeredChannels.end(), channel->Channel());
				if (found == triggeredChannels.end()) {
					triggeredChannels.push_back(channel->Channel());
				}
			});

			OnChannelClear([&](Ref<DiscreteInputChannel> channel) {
				cout << "Removing Channel: " << channel->Channel()<<endl;
				triggeredChannels.erase(std::remove(triggeredChannels.begin(), triggeredChannels.end(), channel->Channel()), triggeredChannels.end());
			});
		}
		void privateLoop();
	};

};

