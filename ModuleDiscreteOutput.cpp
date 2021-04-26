#include "ModuleDiscreteOutput.h"

namespace MonitoringComponents {

	void ModuleDiscreteOutput::Init(bool initial) {
		State initialOutput = (initial) ? State::High : State::Low;
		this->setOutput(initialOutput);
	}

	void ModuleDiscreteOutput::setOutput(State outputState) {
		this->state = outputState;
		int outputValue = (outputState == State::High) ? 1 : 0;
		P1.writeDiscrete(outputValue, this->_address.Slot, this->_address.Channel);
	}

	bool ModuleDiscreteOutput::isHigh() {
		//P1.readDiscrete() --test if you can read current output state.  Easier than storing it internally.  Or may need both
		return this->state == State::High;
	}

	bool ModuleDiscreteOutput::isLow() {
		return !this->isHigh();
	}
};