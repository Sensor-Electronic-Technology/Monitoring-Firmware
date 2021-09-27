#pragma once

#include "MonitoringComponent.h"
#include "ModbusService.h"
#include "P1AM.h"
#include "Ref.h"
#include "ModuleDiscreteOutput.h"
#include "Configuration.h"

namespace MonitoringComponents {
	class DiscreteOutputChannel :public MonitoringComponent {
	public:

		DiscreteOutputChannel():MonitoringComponent(){	}

		DiscreteOutputChannel(OutputConfiguration configuration, Ref<MonitoringComponent> parent = nullptr)
			:MonitoringComponent(parent), configuration(configuration),
			outputPin(configuration.address),
			modbusAddress({configuration._register,RegisterType::Input}) {
			this->initialState = this->configuration.startState;
		}

		ChannelAddress Address() {
			return this->outputPin.Address();
		}

		void Init();
		void Init(State initial);
		void SetOutput(State level);
		bool OutputState();
		
	private:
		ModuleDiscreteOutput outputPin;
		OutputConfiguration configuration;
		ModbusAddress modbusAddress;
		State initialState;
		void privateLoop();
	};
};


