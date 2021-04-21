#pragma once

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
			modbusAddress({ configuration._register,RegisterType::DiscreteInput }), _on_trigger([]() {})
		{
			this->inputPin = ModuleDiscreteInput(configuration.slot, configuration.channel);
			this->triggerOn = (this->configuration.Logic == LogicType::High) ? TriggerOn::High : TriggerOn::Low;
			this->_triggered = false;
		}

		DiscreteInputChannel() :MonitoringComponent(nullptr), _on_trigger([]() {}) {	}

		bool isTriggered();

		void OnTrigger(VoidCallback cbk);

	private:
		ModuleDiscreteInput inputPin;
		ModbusAddress modbusAddress;
		TriggerOn triggerOn;
		bool _triggered;
		DigitalInConfiguration configuration;

		VoidCallback _on_trigger;

		void privateLoop();
	};
};