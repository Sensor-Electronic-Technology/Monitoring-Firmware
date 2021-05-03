#include "DiscreteInputChannel.h"

namespace MonitoringComponents {
	bool DiscreteInputChannel::isTriggered() {
		if (this->configuration.connected) {		
			return (this->triggerOn == TriggerOn::Low) ? this->inputPin.isLow() : this->inputPin.isHigh();
		}
		return false;
	}

	void DiscreteInputChannel::Initialize() {
		bool state = this->isTriggered();
		if (state) {
			ChannelMessage msg;
			//msg.channel.slot = this->configuration.slot;
			//msg.channel.channel = this->configuration.channel;
			//msg.action = ChannelAction::Trigger;
			this->_on_state_change(msg);
		}
		this->triggered = state;
	}

	int DiscreteInputChannel::Channel() {
		return this->configuration.channel;
	}

	void DiscreteInputChannel::OnStateChange(ChannelCallback cbk) {
		this->_on_state_change = cbk;
	}

	void DiscreteInputChannel::privateLoop() {
		bool state = this->isTriggered();
		if (state != this->triggered) {
			ChannelMessage message;
			//message.channel.channel = this->configuration.channel;
			//message.channel.slot = this->configuration.slot;
			//if (state) {
			//	message.action = ChannelAction::Trigger;
			//}else {
			//	message.action = ChannelAction::Clear;
			//}
			this->triggered = state;
			this->_on_state_change(message);
		}
	}
};

