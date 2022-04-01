#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {	
		int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		return CurrentMax * ((float)counts / Bit13Reg);
	}
};