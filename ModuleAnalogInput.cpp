#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {
		float counts= P1.readAnalog(this->_address.slot, this->_address.channel);
		float val= CurrentMax * ((float)counts/Bit13Reg);
		this->value = val;
		//int counts= P1.readAnalog(this->_address.slot, this->_address.channel);
		//std::cout << "{" << this->_address.slot << "," << this->_address.channel <<"}="<<val<<std::endl;
		return val;
	}

	float ModuleAnalogInput::getValue() {
		return this->value;
	}
};