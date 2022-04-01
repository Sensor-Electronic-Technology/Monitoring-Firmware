#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {	
		int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		this->currentValue+=((CurrentMax * ((float)counts / Bit13Reg))-this->currentValue)*fWeight;
		return this->currentValue;
	}
};