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

		ChannelMessage(ChannelAction cAction,int id,ActionType aType,ChannelAddress ch)
			:channelAction(cAction),actionId(id),type(aType),channel(ch) { }

		ChannelMessage() {
			this->channelAction = ChannelAction::Clear;
			this->actionId = -1;
			this->type = ActionType::Okay;
		}

		ChannelMessage& operator=(const ChannelMessage& msg) {
			this->channelAction = msg.channelAction;
			this->actionId = msg.actionId;
			this->type = msg.type;
			this->channel = msg.channel;
		}
	};

	typedef Function<void(void)> VoidCallback;
	typedef Function<void(Ref<DiscreteInputChannel>)> DiscreteInputCallback;
	typedef Function<void(Ref<AnalogInputChannel>)> AnalogInputCallback;

	typedef Function<void(ChannelMessage)> ChannelCallback;
};