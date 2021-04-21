#include "DiscreteInputChannel.h"

namespace MonitoringComponents {
	bool DiscreteInputChannel::isTriggered() {
		Serial.println("In isTriggerd()");
		if (this->configuration.connected) {
			Serial.println("Is Connected==true");
			this->_triggered = (this->triggerOn == TriggerOn::Low) ? this->inputPin.isLow() : this->inputPin.isHigh();
			Serial.println("Triggered: " + String(this->_triggered));
			return this->_triggered;
		}
		Serial.println("Connected==false");
		this->_triggered = false;
		Serial.println("Leaving isTriggered()");
		return false;
	}

	void DiscreteInputChannel::OnTrigger(VoidCallback cbk) {
		this->_on_trigger = cbk;
	}

	void DiscreteInputChannel::privateLoop() {
		bool triggered = this->isTriggered();
		if (triggered) {
			this->_on_trigger();
		}
	}
};

