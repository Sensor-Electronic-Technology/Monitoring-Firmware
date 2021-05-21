#pragma once

#include "MonitoringComponent.h"
#include "ModbusService.h"
#include "P1AM.h"
#include "Ref.h"
#include "Callbacks.h"
#include "ModuleAnalogInput.h"
#include "Configuration.h"
#include "Data.h"
#include "ChannelAlert.h"

namespace MonitoringComponents {

	class AnalogInputChannel :public MonitoringComponent {
	public:
		AnalogInputChannel(AnalogInConfiguration configuration, Ref<MonitoringComponent> parent = nullptr) 
			:MonitoringComponent(parent), configuration(configuration), _on_channel_trigger([](ChannelMessage){}),
			modbusAddress({configuration._register,RegisterType::Input}) { 
			inputPin = ModuleAnalogInput(configuration.address);
			this->alert1 = configuration.alert1;
			this->alert2 = configuration.alert2;
			this->alert3 = configuration.alert3; 
		}
		AnalogInputChannel() :_on_channel_trigger([](ChannelMessage) {}) {	}
		void Initialize();
		bool isTriggered();
		void OnStateChange(ChannelCallback cbk);
	private:
		ModuleAnalogInput inputPin;
		ModbusAddress modbusAddress;
		AnalogInConfiguration configuration;

		AnalogAlert alert1;
		AnalogAlert alert2;
		AnalogAlert alert3;


		float value;
		bool triggered;
		ChannelCallback _on_channel_trigger;
		void privateLoop();
	};
};


