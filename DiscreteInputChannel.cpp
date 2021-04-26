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
			this->_on_trigger(this);
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

	void DiscreteInputChannel::privateLoop() {
		bool state = this->isTriggered();
		if (state != this->_triggered) {
			if (state) {
				this->_on_trigger(this);
			}else {
				this->_on_clear(this);
			}
			this->_triggered = state;
		}
	}
};

