#pragma once
#include <ArduinoSTL.h>
#include "P1AM.h"
#include "IO.h"
#include "Global.h"
#include "MonitoringLogger.h"

#define Bit13Reg			8191
#define CurrentMax			20.0f
#define CurrentMin			4.00f
#define fWeight				.1f

namespace MonitoringComponents {
	class ModuleAnalogInput :public ModuleIOPin {
	public:
		ModuleAnalogInput(ChannelAddress address):ModuleIOPin(address){
			const char config[] = { 0x40, 0x07 };
			#if IO_DEBUG==1
				MonitoringLogger::LogDebug(F("ModuleAnalogInput Configured"));
			#else
				P1.configureModule(config, this->_address.slot);
			#endif
		}

		ModuleAnalogInput(int slot, int channel) :ModuleIOPin(slot, channel){}
		ModuleAnalogInput() :ModuleIOPin(0, 0) {}
		float read();
	private:
		float currentValue=0.00f;
	};
};
