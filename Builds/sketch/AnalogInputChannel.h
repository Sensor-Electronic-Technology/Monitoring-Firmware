#pragma once

#include "MonitoringComponent.h"
#include "Timer.h"
#include "ModbusService.h"
#include "P1AM.h"
#include "Ref.h"
#include "Callbacks.h"
#include "ModuleAnalogInput.h"
#include "Configuration.h"
#include "Data.h"
#include "ChannelAlert.h"

#define fWeight			.01f
#define UPDATEPERIOD	100
#define READPERIOD		50

namespace MonitoringComponents {

	class AnalogInputChannel :public MonitoringComponent {
	public:
		AnalogInputChannel(AnalogInConfiguration configuration, Ref<MonitoringComponent> parent = nullptr) 
			:MonitoringComponent(parent), configuration(configuration), _on_channel_trigger([](ChannelMessage){}),
			modbusAddress({configuration._register,RegisterType::Input}),
			alertModAddress({configuration.alertModAddr,RegisterType::Holding}) { 
			inputPin = ModuleAnalogInput(configuration.address);

			this->alert1 = configuration.alert1;
			this->alert2 = configuration.alert2;
			this->alert3 = configuration.alert3; 
		}

		AnalogInputChannel() :_on_channel_trigger([](ChannelMessage) {}) {
			this->value = 0;
		}

		void Initialize();
		void CheckProcessAlerts();
		void OnStateChange(ChannelCallback cbk);
		void Read();
	private:
		ModuleAnalogInput inputPin;
		ModbusAddress modbusAddress;
		ModbusAddress alertModAddress;
		AnalogInConfiguration configuration;

		AnalogAlert alert1;
		AnalogAlert alert2;
		AnalogAlert alert3;

		Timer updateTimer;
		Timer readTimer;

		float value;
		ChannelCallback _on_channel_trigger;
		void privateLoop();
	};
};


