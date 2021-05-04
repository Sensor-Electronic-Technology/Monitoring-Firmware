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
		//if (this->alert.enabled && !this->alert.bypass) {
		if (state) {
			ChannelMessage msg;
			msg.actionId = this->alert.actionId;
			msg.channel = this->inputPin.Address();
			msg.channelAction = ChannelAction::Trigger;
			this->alert.activated = true;
			this->_on_state_change(msg);
		}
		//}
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
		if (state!=this->alert.activated) {
			ChannelMessage message;
			message.actionId = this->alert.actionId;
			message.channel = this->inputPin.Address();			
			if (state) {
				message.channelAction = ChannelAction::Trigger;
				this->alert.activated = true;
			}else {
				message.channelAction = ChannelAction::Clear;
				this->alert.activated = false;
			}
			this->triggered = state;

			this->_on_state_change(message);
		}
	}
};

