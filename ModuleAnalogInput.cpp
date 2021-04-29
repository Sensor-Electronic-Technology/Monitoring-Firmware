#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {
		int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		this->value = CurrentMax * ((float)counts, Bit13Reg);
		return this->value;
	}

	float ModuleAnalogInput::getValue() {
		return this->value;
	}
};