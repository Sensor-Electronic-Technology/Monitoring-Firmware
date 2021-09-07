#pragma once
#include <ArduinoSTL.h>
#include "P1AM.h"
#include "IO.h"
#include "Global.h"

#define Bit13Reg			8191
#define CurrentMax			20

namespace MonitoringComponents {
	class ModuleAnalogInput :public ModuleIOPin {
	public:
		ModuleAnalogInput(ChannelAddress address):ModuleIOPin(address){
			const char config[] = { 0x40, 0x07 };
			#if IO_DEBUG==1
				std::cout << "ModuleAnalogInput Configured" << std::endl;
			#else
				P1.configureModule(config, this->_address.slot);
			#endif
		}

		ModuleAnalogInput(int slot, int channel) :ModuleIOPin(slot, channel), value(0.00){}
		ModuleAnalogInput() :ModuleIOPin(0, 0),value(0.00) {}
		float read();
		float getValue();
	private:
		float value;
	};
};
