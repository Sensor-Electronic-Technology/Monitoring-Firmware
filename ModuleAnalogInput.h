#pragma once
#include "P1AM.h"
#include "IO.h"

#define Bit13Reg			8191
#define CurrentMax			20

namespace MonitoringComponents {
	class ModuleAnalogInput :public ModuleIOPin {
	public:
		ModuleAnalogInput(PinAddress address):ModuleIOPin(address){}

		ModuleAnalogInput(int slot, int channel) :ModuleIOPin(slot, channel), value(0.00){}

		ModuleAnalogInput() :ModuleIOPin(0, 0),value(0.00) {}
		
		float read();
		
		float getValue();

	private:
		float value;
	};
};
