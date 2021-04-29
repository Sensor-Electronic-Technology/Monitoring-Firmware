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
		bool val = (bool)P1.readDiscrete(this->_address.slot, this->_address.channel);
		this->state = (val == true) ? State::High : State::Low;
		//this->state = (this->state == State::High) ? State::Low : State::High;
	}
};