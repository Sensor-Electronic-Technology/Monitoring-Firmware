#pragma once

#include "MonitoringComponent.h"
#include "P1AM.h"
#include "Ref.h"
#include "ModuleDiscreteOutput.h"
#include "Configuration.h"

namespace MonitoringComponents {
	class DiscreteOutputChannel :public MonitoringComponent {
	public:
		DiscreteOutputChannel(OutputConfiguration configuration, Ref<MonitoringComponent> parent = nullptr)
			:MonitoringComponent(parent), configuration(configuration),
			outputPin(configuration.channel, configuration.slot),
			modbusAddress({ configuration._register,RegisterType::Input }) {
			//this->triggerOn = (this->configuration.Logic == LogicType::High) ? TriggerOn::High : TriggerOn::Low;
			this->triggered = false;
		}

		void Print() {
			//cout << "Channel: " << this->configuration.channel;
			//cout << " Slot: " << this->configuration.slot;
			//cout << " Register: " << this->configuration._register << endl;
		}

		void Init();
		bool isOn();
		
	private:
		ModuleDiscreteOutput outputPin;
		OutputConfiguration configuration;
		ModbusAddress modbusAddress;
		//TriggerOn triggerOn;
		bool triggered;

		void privateLoop();
	};
};


