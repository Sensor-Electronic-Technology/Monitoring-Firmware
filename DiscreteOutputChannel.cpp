#include "DiscreteOutputChannel.h"

namespace MonitoringComponents {

	void DiscreteOutputChannel::Init() {
		bool initial = (this->configuration.startState == LogicType::High) ? true : false;
		this->outputPin.Init(initial);
	}

	bool DiscreteOutputChannel::isOn() {
		return (this->triggerOn == TriggerOn::High) ? this->outputPin.isHigh() : this->outputPin.isLow();
	}

	void DiscreteOutputChannel::TurnOn() {
		if (!this->isOn()) {
			if (this->triggerOn == TriggerOn::High) {
				this->outputPin.setOutput(State::High);
			} else {
				this->outputPin.setOutput(State::Low);
			}
		}
	}

	void DiscreteOutputChannel::privateLoop() {

	}
};

