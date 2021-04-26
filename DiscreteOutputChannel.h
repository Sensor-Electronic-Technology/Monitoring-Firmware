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
			this->triggerOn = (this->configuration.triggerOn == LogicType::High) ? TriggerOn::High : TriggerOn::Low;
			this->outputOn = false;
			this->initialState = (this->configuration.startState == LogicType::High) ? State::High : State::Low;
		}
		void Init();
		void TurnOn();
		bool isOn();
		
	private:
		ModuleDiscreteOutput outputPin;
		OutputConfiguration configuration;
		ModbusAddress modbusAddress;
		TriggerOn triggerOn;
		State initialState;
		bool outputOn;
		void privateLoop();
	};
};


