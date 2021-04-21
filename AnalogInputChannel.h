#pragma once

#include "MonitoringComponent.h"
#include  "P1AM.h"
#include "Ref.h"
#include "Callbacks.h"
#include "ModuleAnalogInput.h"
#include "Configuration.h"

namespace MonitoringComponents {
	class AnalogInputChannel :public MonitoringComponent {
	public:
		AnalogInputChannel(AnalogInConfiguration configuration, Ref<MonitoringComponent> parent = nullptr) 
			:MonitoringComponent(parent), configuration(configuration), _on_trigger([]() {}),
			modbusAddress({configuration._register,RegisterType::Holding}) 
		{ 
			inputPin = ModuleAnalogInput(configuration.slot, configuration.channel);
		}

		AnalogInputChannel() :_on_trigger([]() {}) {

		}

		void OnTrigger(VoidCallback cbk);

	private:
		ModuleAnalogInput inputPin;
		ModbusAddress modbusAddress;
		AnalogInConfiguration configuration;
		bool triggered;
		VoidCallback _on_trigger;

		void privateLoop();
	};
};


