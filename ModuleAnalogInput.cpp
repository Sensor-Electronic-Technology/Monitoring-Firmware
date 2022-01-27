#include "ModuleAnalogInput.h"

namespace MonitoringComponents {
	float ModuleAnalogInput::read() {
		float val = 0.00;
#if IO_DEBUG==1
		this->value = 20;
		val = 20;
#else
/* 		float counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		 val= CurrentMax * ((float)counts / Bit13Reg); */
		
		int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		val= CurrentMax * ((float)counts / Bit13Reg);
		this->value = val;
/* 		for(int i=0;i<10;i++){
			int counts = P1.readAnalog(this->_address.slot, this->_address.channel);
		 	val+= CurrentMax * ((float)counts / Bit13Reg);
		}
		this->value = val/10; */
#endif
		return val;
	}

	float ModuleAnalogInput::getValue() {
		return this->value;
	}
};