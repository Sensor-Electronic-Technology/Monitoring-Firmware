#pragma once
#include <ArduinoSTL.h>
#include "MonitoringComponent.h"
#include "ModuleDiscreteOutput.h"
#include "DiscreteOutputChannel.h"
namespace MonitoringComponents {

	class ActionOutput {
	public:
		ActionOutput(){}
		void Set(ChannelAddress address, State level);
		void Initialize();
		void ConfigureChannel(OutputConfiguration outputConfig);
		void Invoke();

		ActionOutput& operator=(const ActionOutput& other) {

		}

	private:
		ChannelAddress channelAddress;
		State level;
		DiscreteOutputChannel channel;
	};
};


