#include "DiscreteOutputChannel.h"

namespace MonitoringComponents {

	void DiscreteOutputChannel::Init() {
		this->outputPin.Init(this->initialState);
	}

	void DiscreteOutputChannel::Init(State initial) {
		this->initialState = initial;
		ChannelAddress addr = this->outputPin.Address();
		this->outputPin.Init(this->initialState);
	}

	void DiscreteOutputChannel::SetOutput(State state) {	
		ModbusService::Update(this->modbusAddress,(bool)state);
		this->outputPin.setOutput(state);
	}

	bool DiscreteOutputChannel::OutputState(){
		return this->outputPin.isHigh();
	}

	void DiscreteOutputChannel::privateLoop() {

	}
};

