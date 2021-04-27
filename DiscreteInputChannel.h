#pragma once
#include <ArduinoSTL.h>
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
			modbusAddress({ configuration._register,RegisterType::DiscreteInput }), _on_trigger([](Ref<DiscreteInputChannel>) {}), _on_clear([](Ref<DiscreteInputChannel>) {}), _on_ch_trigger([](ChannelMessage) {}) {
			this->inputPin = ModuleDiscreteInput(configuration.slot, configuration.channel);
			this->triggerOn = (this->configuration.Logic == LogicType::High) ? TriggerOn::High : TriggerOn::Low;
			this->_triggered = false;
		}

		DiscreteInputChannel() :MonitoringComponent(nullptr), 
			_on_trigger([](Ref<DiscreteInputChannel>) { }), 
			_on_clear([](Ref<DiscreteInputChannel>) { }),
			_on_ch_trigger([](ChannelMessage){ }) {	}

		bool isTriggered();

		int Channel();

		void OnTrigger(DiscreteInputCallback cbk);

		void OnClear(DiscreteInputCallback cbk);

		void OnTrigger(ChannelCallback cbk);

		void Initialize();

	private:
		ModuleDiscreteInput inputPin;
		ModbusAddress modbusAddress;
		TriggerOn triggerOn;
		bool _triggered;
		DigitalInConfiguration configuration;
		DiscreteInputCallback _on_clear;
		DiscreteInputCallback _on_trigger;
		ChannelCallback		  _on_ch_trigger;

		void privateLoop();
	};
};