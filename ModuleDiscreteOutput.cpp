#include "ModuleDiscreteOutput.h"

namespace MonitoringComponents {

	void ModuleDiscreteOutput::Init(State initial) {
		State initialOutput = initial;
		this->setOutput(initialOutput);
	}

	void ModuleDiscreteOutput::setOutput(State outputState) {
		this->state = outputState;
		int outputValue = (outputState == State::High) ? 1 : 0;
#if IO_DEBUG==1
		//std::cout << "Set Output " << "{" << this->_address.channel << "," << this->_address.slot << "} " << outputValue << std::endl;
#else
		P1.writeDiscrete(outputValue, this->_address.slot, this->_address.channel);
#endif
	}

	bool ModuleDiscreteOutput::isHigh() {
		//P1.readDiscrete() --test if you can read current output state.  Easier than storing it internally.  Or may need both
		return this->state == State::High;
	}

	bool ModuleDiscreteOutput::isLow() {
		return !this->isHigh();
	}
};