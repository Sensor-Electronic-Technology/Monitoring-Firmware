#include "DiscreteOuputChannel.h"

namespace MonitoringComponents {

	void DiscreteOutputChannel::Init() {
		this->outputPin.Init();
	}

	bool DiscreteOutputChannel::isOn() {
		//return (this->triggerOn == TriggerOn::High) ? this->outputPin.isHigh() : this->outputPin.isLow();
		return false;
	}

	void DiscreteOutputChannel::privateLoop() {
		//do nothing
	}
};

