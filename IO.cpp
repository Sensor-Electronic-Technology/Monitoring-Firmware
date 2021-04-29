#include "IO.h"

namespace MonitoringComponents {

	ModuleIOPin::ModuleIOPin(int slot, int channel) {
		this->_address.slot = slot;
		this->_address.channel = channel;
	}

	ChannelAddress ModuleIOPin::Address() {
		return this->_address;
	}
};
