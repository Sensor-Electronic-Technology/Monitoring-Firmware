#pragma once
#include "Ref.h"
#include "Function.h"
#include "Callbacks.h"
#include "Configuration.h"

namespace MonitoringComponents {
	class DiscreteInputChannel;
	class AnalogInputChannel;

	struct ChannelMessage {
		ChannelAction channelAction;
		int actionId;
		ActionType type;
		ChannelAddress channel;


	};

	typedef Function<void(void)> VoidCallback;
	typedef Function<void(Ref<DiscreteInputChannel>)> DiscreteInputCallback;
	typedef Function<void(Ref<AnalogInputChannel>)> AnalogInputCallback;

	typedef Function<void(ChannelMessage)> ChannelCallback;
};