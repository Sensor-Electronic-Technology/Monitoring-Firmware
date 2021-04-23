#include "DiscreteInputChannel.h"

namespace MonitoringComponents {
	bool DiscreteInputChannel::isTriggered() {
		if (this->configuration.connected) {
			this->_triggered = (this->triggerOn == TriggerOn::Low) ? this->inputPin.isLow() : this->inputPin.isHigh();
			return this->_triggered;
		}
		this->_triggered = false;
		return false;
	}

	void DiscreteInputChannel::OnTrigger(DiscreteInputCallBack cbk) {
		this->_on_trigger = cbk;
	}

	void DiscreteInputChannel::privateLoop() {
		this->_on_trigger(this);
	}
};

