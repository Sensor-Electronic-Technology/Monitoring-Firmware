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
		bool operator==(DistinctChannel ch) {
			return this->channel == ch.channel && this->slot == ch.slot;
		}
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
		bool operator==(const ChannelMessage& other) {
			return this->channel==other.channel && this->action==other.action;
		}
	};

	typedef Function<void(void)> VoidCallback;
	typedef Function<void(Ref<DiscreteInputChannel>)> DiscreteInputCallback;
	typedef Function<void(Ref<AnalogInputChannel>)> AnalogInputCallback;

	typedef Function<void(ChannelMessage)> ChannelCallback;
};