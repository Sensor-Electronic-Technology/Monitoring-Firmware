#pragma once
#include "Ref.h"
#include "Function.h"
#include "Callbacks.h"
#include "Configuration.h"

namespace MonitoringComponents {
	class DiscreteInputChannel;
	class AnalogInputChannel;

	struct DistinctChannel {
		int channel;
		int slot;
	};

	enum class ChannelAction {
		Clear,Trigger
	};

	struct AlertMessage {
		AlertAction action;
		DistinctChannel channel;
	};

	struct ChannelMessage {
		DistinctChannel channel;
		ChannelAction action;
	};

	typedef Function<void(void)> VoidCallback;
	typedef Function<void(Ref<DiscreteInputChannel>)> DiscreteInputCallback;
	typedef Function<void(Ref<AnalogInputChannel>)> AnalogInputCallback;

	typedef Function<void(ChannelMessage)> ChannelCallback;
};