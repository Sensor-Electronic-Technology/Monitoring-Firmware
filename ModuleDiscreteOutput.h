#pragma once
#include <ArduinoSTL.h>
#include "P1AM.h"
#include "IO.h"


namespace MonitoringComponents {
	class ModuleDiscreteOutput :public ModuleIOPin {
	public:
		ModuleDiscreteOutput(ChannelAddress address, State defaultState) :ModuleIOPin(address), state(defaultState) {}
		ModuleDiscreteOutput(ChannelAddress address):ModuleDiscreteOutput({ 0,0 }, State::Low) {}
		ModuleDiscreteOutput(int slot,int channel):ModuleIOPin(slot,channel),state(State::Low){}
		//ModuleDiscreteOutput(int slot,int channel,State defaultState):ModuleIOPin(slot, channel), state(defaultState){}
		ModuleDiscreteOutput() :ModuleDiscreteOutput({0,0},State::Low) {}
		void Init(State initial);
		void setOutput(State outputState);
		bool isHigh();
		bool isLow();
	private:
		State state;
	};
};
