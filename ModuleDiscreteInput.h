#pragma once
#include <ArduinoSTL.h>
#include "IO.h"
#include "P1AM.h"

namespace MonitoringComponents {
	class ModuleDiscreteInput :public ModuleIOPin {
	public:
		ModuleDiscreteInput(ChannelAddress address) :ModuleIOPin(address),state(State::Low){}

		ModuleDiscreteInput(int slot,int channel):ModuleIOPin(slot,channel), state(State::Low){}

		ModuleDiscreteInput(int slot, int channel, State defaultState) :ModuleIOPin(slot, channel), state(defaultState) {}

		ModuleDiscreteInput() :ModuleIOPin(0, 0), state(State::Low) {}	

		bool isHigh();
		bool isLow();
	private:
		void read();
		State state;
	};
};

