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

#define fWeight			.1f
#define UPDATEPERIOD	50
#define READPERIOD		50

namespace MonitoringComponents {

	class AnalogInputChannel :public MonitoringComponent {
	public:
		AnalogInputChannel(AnalogInConfiguration configuration, Ref<MonitoringComponent> parent = nullptr) 
			:MonitoringComponent(parent), configuration(configuration),
			 _on_channel_trigger([](ChannelMessage){}),
			modbusAddress(configuration._modbusAddress),
			alertModAddress(configuration.alertAddress),
			alert1(configuration.alert1),
			alert2(configuration.alert2),
			alert3(configuration.alert3),
			value(0.00f) { 
			inputPin = ModuleAnalogInput(configuration.address);
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


