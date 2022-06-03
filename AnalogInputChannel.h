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
#define UPDATEPERIOD	100
#define MinCurrent		4.00f
#define MaxCurrent		20.00F

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
			analogFactor(configuration.analogFactor) { 
			inputPin = ModuleAnalogInput(configuration.address);
			this->maxSensor=MaxCurrent*configuration.slope+configuration.offset;
			this->minSensor=MinCurrent*configuration.slope+configuration.offset;
		}

		AnalogInputChannel() :_on_channel_trigger([](ChannelMessage) {}) {
			this->analogFactor=10;
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
		int16_t analogFactor;
		float maxSensor=0.00f;
		float minSensor=0.00f;

		AnalogAlert alert1;
		AnalogAlert alert2;
		AnalogAlert alert3;

		Timer updateTimer;
		Timer readTimer;
		float sensorValue=0.00f;
		float currentValue=0.00f;
		ChannelCallback _on_channel_trigger;
		void privateLoop();
	};
};


