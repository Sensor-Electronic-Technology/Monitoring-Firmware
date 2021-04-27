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
			msg.channel.slot = this->configuration.slot;
			msg.channel.channel = this->configuration.channel;
			msg.action = ChannelAction::Trigger;
			this->_on_ch_trigger(msg);
		}
		this->_triggered = state;
	}

	int DiscreteInputChannel::Channel() {
		return this->configuration.channel;
	}

	void DiscreteInputChannel::OnTrigger(DiscreteInputCallback cbk) {
		this->_on_trigger = cbk;
	}

	void DiscreteInputChannel::OnClear(DiscreteInputCallback cbk) {
		this->_on_clear=cbk;
	}

	void DiscreteInputChannel::OnTrigger(ChannelCallback cbk) {
		this->_on_ch_trigger = cbk;
	}

	void DiscreteInputChannel::privateLoop() {
		bool state = this->isTriggered();
		if (state != this->_triggered) {
			ChannelMessage message;
			message.channel.channel = this->configuration.channel;
			message.channel.slot = this->configuration.slot;
			if (state) {
				message.action = ChannelAction::Trigger;
				//this->_on_trigger(this);
			}else {
				message.action = ChannelAction::Clear;
				//this->_on_clear(this);
			}
			this->_triggered = state;
			this->_on_ch_trigger(message);
		}
	}
};

