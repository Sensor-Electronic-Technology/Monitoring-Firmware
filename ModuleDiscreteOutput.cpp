#include "ModuleDiscreteOutput.h"

namespace MonitoringComponents {

	void ModuleDiscreteOutput::Init(State initial) {
		State initialOutput = initial;
		this->setOutput(initialOutput);
	}

	void ModuleDiscreteOutput::setOutput(State outputState) {
		this->state = outputState;
		int outputValue = (outputState == State::High) ? 1 : 0;
		std::cout << "Set Output " << "{" << this->_address.channel << "," << this->_address.slot << "} " <<outputValue<<std::endl;
		P1.writeDiscrete(outputValue, this->_address.slot, this->_address.channel);
	}

	bool ModuleDiscreteOutput::isHigh() {
		//P1.readDiscrete() --test if you can read current output state.  Easier than storing it internally.  Or may need both
		return this->state == State::High;
	}

	bool ModuleDiscreteOutput::isLow() {
		return !this->isHigh();
	}
};