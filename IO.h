#pragma once
#include "Data.h"

namespace MonitoringComponents {
	class ModuleIOPin {
	public:
		ModuleIOPin(ChannelAddress addr):_address(addr){}
		ModuleIOPin(int slot, int channel);
		ChannelAddress Address();
	protected:
		ChannelAddress _address;
	};
};





