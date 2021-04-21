#pragma once

namespace MonitoringComponents {
	struct PinAddress {
		int Slot;
		int Channel;
	};

	class ModuleIOPin {
	public:
		ModuleIOPin(PinAddress addr):_address(addr){}
		ModuleIOPin(int slot, int channel);
	protected:
		PinAddress _address;
	};

	enum class State : bool {
		High = true,
		Low = false
	};

	enum class TriggerOn {
		High,
		Low
	};
};





