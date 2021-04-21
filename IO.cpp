#include "IO.h"

namespace MonitoringComponents {
	ModuleIOPin::ModuleIOPin(int slot, int channel) {
		this->_address.Slot = slot;
		this->_address.Channel = channel;
	}
};
