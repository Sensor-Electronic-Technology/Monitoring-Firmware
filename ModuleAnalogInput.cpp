#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {	
		int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		counts=constrain(counts,0,Bit13Reg);
		float calc=(CurrentMax * ((float)counts / Bit13Reg));
		calc=constrain(calc,CurrentMin,CurrentMax);
		this->currentValue+=(calc-this->currentValue)*fWeight;
		return this->currentValue;
	}
};