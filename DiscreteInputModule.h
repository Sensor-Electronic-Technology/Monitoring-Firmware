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
			:IModule(configuration,parent), _on_channel_trigger([](Ref<DiscreteInputChannel>) {}),
			_on_channel_clear([](Ref<DiscreteInputChannel>) { }),
			_on_ch_trigger([](ChannelMessage){ }) {
			this->BuildCallbacks();
		}

		DiscreteInputModule(Ref<MonitoringComponent> parent = nullptr) 
			:IModule(parent),
			_on_channel_trigger([](Ref<DiscreteInputChannel>) {}), 
			_on_channel_clear([](Ref<DiscreteInputChannel>) {}), 
			_on_ch_trigger([](ChannelMessage) {}) {
			this->BuildCallbacks();
		}

		int Slot();
		void CreateChannels(std::vector<DigitalInConfiguration> channelConfigurations);
		void SetConfig(ModuleConfiguration config);
		void Initialize();
		void OnChannelTrigger(DiscreteInputCallback cbk);
		void OnChannelClear(DiscreteInputCallback cbk);
		void OnChTrigger(ChannelCallback cbk);
		
	private:
		std::vector<DiscreteInputChannel*> channels;
		std::vector<DistinctChannel> triggeredChannels;
		Initialized isInitialized;
		DiscreteInputCallback _on_channel_trigger;
		DiscreteInputCallback _on_channel_clear;
		ChannelCallback		  _on_ch_trigger;
		Timer timer;

		void BuildCallbacks() {
			OnChannelTrigger([&](Ref<DiscreteInputChannel> channel) {
				//cout << "Channel " << channel->Channel() << " Triggered" << endl;
				//auto found = std::find(triggeredChannels.begin(),triggeredChannels.end(), channel->Channel());
				//if (found == triggeredChannels.end()) {
				//	triggeredChannels.push_back(channel->Channel());
				//}
			});

			OnChannelClear([&](Ref<DiscreteInputChannel> channel) {
				//cout << "Removing Channel: " << channel->Channel()<<endl;
				//triggeredChannels.erase(std::remove(triggeredChannels.begin(), triggeredChannels.end(), channel->Channel()), triggeredChannels.end());
			});

			OnChTrigger([&](ChannelMessage message) {
				if (message.action == ChannelAction::Clear) {
					auto found = std::find_if(triggeredChannels.begin(), triggeredChannels.end(), [&](DistinctChannel ch) {
							return ch== message.channel;
					});
					if (found == triggeredChannels.end()) {
						triggeredChannels.push_back(message.channel);
					}
				} else if (message.action == ChannelAction::Trigger) {
					triggeredChannels.erase(std::remove_if(triggeredChannels.begin(), triggeredChannels.end(), [&](DistinctChannel ch) {
						return ch == message.channel;
						}), triggeredChannels.end());
				}//else do nothing
			});
		}
		void privateLoop();
	};

};

