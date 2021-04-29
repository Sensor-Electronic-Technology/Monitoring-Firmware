#pragma once
#include <ArduinoSTL.h>
#include <map>
#include "Timer.h"
#include "Configuration.h"
#include "ConfigurationReader.h"

namespace MonitoringComponents {

	enum class ControllerState {
		Alarming,Warning,Maintenance,Okay
	};

	class MonitoringController:public MonitoringComponent {
	public:
		MonitoringController() :MonitoringComponent(), _on_channel_cbk([](ChannelMessage) {}){}
		void Run();
		void BuildChannels();
		void OnChannelCallback(ChannelCallback cbk); 
	private:
		std::vector<DiscreteInputChannel*> discreteInputs;
		std::vector<AnalogInputChannel*> analogInputs;
		std::vector<DiscreteOutputChannel*> outputChannels;
		std::map<ChannelAction, std::vector<ChannelAddress>> triggeredMap;
		std::map<ChannelAction, int> actionMap;
		ChannelCallback _on_channel_cbk;

		ControllerState controllerState;
		Timer resetTimer;
	};
};



