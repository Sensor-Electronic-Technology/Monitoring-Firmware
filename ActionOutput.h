#pragma once
#include <ArduinoSTL.h>
#include "MonitoringComponent.h"
#include "DiscreteOutputChannel.h"

namespace MonitoringComponents {

	class ActionOutput {
	public:
		ActionOutput(){}

		ActionOutput(DiscreteOutputChannel* output, State level) {
			this->channel = output;
			this->level = level;
		}

		void ConfigureChannel(DiscreteOutputChannel* channel) {
			this->channel = channel;
		}

		void TriggerOutput(){
			this->channel->SetOutput(this->level);
		}

		void Reset() {
			State clearLevel = (this->level == State::High) ? State::Low : State::High;
			this->channel->SetOutput(clearLevel);
		}

		~ActionOutput() {
			//delete this->channel;
		}

	private:
		//ChannelAddress channelAddress;
		State level;
		DiscreteOutputChannel* channel;
	};
};


