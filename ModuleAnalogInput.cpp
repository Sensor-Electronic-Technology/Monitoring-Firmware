#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {
		float val = 0.00;		
		int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		val= CurrentMax * ((float)counts / Bit13Reg);
		this->value = val;
		return val;
	}
};