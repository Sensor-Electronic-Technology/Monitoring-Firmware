#pragma once
#include <ArduinoSTL.h>
#include "ModbusService.h"
#include "ModuleDiscreteInput.h"
#include "Function.h"
#include "IO.h"
#include "Callbacks.h"
#include "MonitoringComponent.h"
#include "Configuration.h"



namespace MonitoringComponents {
	class DiscreteInputChannel :public MonitoringComponent {
	public:

		DiscreteInputChannel(DigitalInConfiguration configuration, Ref<MonitoringComponent> parent = nullptr) :MonitoringComponent(parent), configuration(configuration),
			modbusAddress({ configuration._register,RegisterType::DiscreteInput }), _on_state_change([](ChannelMessage) {}) {
			this->inputPin = ModuleDiscreteInput(configuration.address);
			this->alertModbusAddres=configuration.alertModAddr;
			this->triggerOn = this->configuration.alert.triggerOn;
			this->alert = this->configuration.alert;
			this->alert.activated = false;
			this->triggered = false;
		}

		DiscreteInputChannel() :MonitoringComponent(nullptr),_on_state_change([](ChannelMessage){ }) {	}

		void Initialize();
		bool isTriggered();
		int Channel();
		void OnStateChange(ChannelCallback cbk);
	private:
		ModuleDiscreteInput inputPin;
		ModbusAddress modbusAddress;
		int alertModbusAddres;
		TriggerOn triggerOn;
		bool triggered;
		DigitalAlert alert;
		DigitalInConfiguration configuration;
		ChannelCallback	_on_state_change;
		void privateLoop();
	};
};