#pragma once
#include <ArduinoSTL.h>
#include <map>
#include "Timer.h"
#include "Configuration.h"
#include "ConfigurationReader.h"
#include "Action.h"

namespace MonitoringComponents {

	enum class ControllerState {
		Alarming,Warning,Maintenance,Okay
	};

	class MonitoringController:public MonitoringComponent {
	public:
		MonitoringController() :MonitoringComponent(), _on_channel_cbk([](ChannelMessage) {}){}
		void Setup();
		void Run();
		void Initialize();
		void BuildChannels();
		void OnChannelCallback(ChannelCallback cbk);
	private:
		typedef std::vector<ChannelAddress> Registrations;

		std::vector<DiscreteInputChannel*> discreteInputs;
		std::vector<AnalogInputChannel*> analogInputs;
		std::vector<DiscreteOutputChannel*> outputChannels;
		std::vector<Action*> actions;
		std::map<int,Registrations*> actionTracking;
		ChannelCallback _on_channel_cbk;
		ControllerState controllerState;
		//Timer resetTimer;
		Timer printTimer;

		void privateLoop();

	};
};



