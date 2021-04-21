#include "ModuleDiscreteInput.h"

namespace MonitoringComponents {
	bool ModuleDiscreteInput::isHigh() {
		this->read();
		return (this->state == State::High);
	}

	bool ModuleDiscreteInput::isLow() {
		return !this->isHigh();
	}

	void ModuleDiscreteInput::read() {
		//bool temp = (bool)P1.readDiscrete(this->_address.Slot, this->_address.Channel);
		//this->state = (temp == true) ? State::High : State::Low;
		this->state = (this->state == State::High) ? State::Low : State::High;
	}
};