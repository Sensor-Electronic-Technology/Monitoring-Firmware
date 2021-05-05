#pragma once
#include <ArduinoSTL.h>
#include "MonitoringComponent.h"
#include "DiscreteOutputChannel.h"

namespace MonitoringComponents {

	class ActionOutput {
	public:
		ActionOutput(){}

		ActionOutput(Ref<DiscreteOutputChannel> output, State onLevel,State offLevel) {
			this->channel = output;
			this->OnLevel = onLevel;
			this->OffLevel = offLevel;
		}

		void ConfigureChannel(Ref<DiscreteOutputChannel> channel) {
			this->channel = channel;
		}

		void TriggerOutput(){
			this->channel->SetOutput(this->OnLevel);
		}

		void Reset() {
			this->channel->SetOutput(this->OffLevel);
		}

		~ActionOutput() {
			//delete this->channel;
		}

	private:
		//ChannelAddress channelAddress;
		State OnLevel;
		State OffLevel;
		Ref<DiscreteOutputChannel> channel;
	};
};


