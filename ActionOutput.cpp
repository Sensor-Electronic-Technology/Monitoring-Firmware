#include "ActionOutput.h"

namespace MonitoringComponents {
	void ActionOutput::Set(ChannelAddress address, State level) {
		this->channelAddress = address;
		this->level = level;
	}

	void ActionOutput::Initialize() {
		this->channel.Init();
	}

	void ActionOutput::ConfigureChannel(OutputConfiguration outputConfig) {
		this->channel = DiscreteOutputChannel(outputConfig);
	}

	void ActionOutput::Invoke() {
		this->channel.SetOutput(this->level);
	}
};