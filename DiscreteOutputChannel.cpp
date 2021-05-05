#include "DiscreteOutputChannel.h"

namespace MonitoringComponents {

	void DiscreteOutputChannel::Init() {
		this->outputPin.Init(this->initialState);
	}

	void DiscreteOutputChannel::Init(State initial) {
		this->initialState = initial;
		ChannelAddress addr = this->outputPin.Address();
		std::cout << "Initializing Discrete InputPin: {" << addr.channel << "," << addr.slot << "} " << std::endl;
		this->outputPin.Init(this->initialState);
	}

	void DiscreteOutputChannel::SetOutput(State state) {		
		this->outputPin.setOutput(state);
	}

	bool DiscreteOutputChannel::OutputState(){
		return this->outputPin.isHigh();
	}

	void DiscreteOutputChannel::privateLoop() {

	}
};

