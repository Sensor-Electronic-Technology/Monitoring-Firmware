#pragma once

#include "MonitoringComponent.h"
#include  "P1AM.h"
#include "Ref.h"
#include "Callbacks.h"
#include "ModuleAnalogInput.h"
#include "Configuration.h"

namespace MonitoringComponents {
	

	struct ChannelAlert:public Alert {
		bool activated;
		DistinctChannel channel;

		ChannelAlert() {
			this->activated = false;
		}

		ChannelAlert(const Alert& alert, DistinctChannel channel) {
			Alert::operator=(alert);
			this->channel = channel;
			this->activated = false;
		}

		ChannelAlert& operator=(const Alert& alert) {
			Alert::operator=(alert);
			this->activated = false;
		}
	};

	class AnalogInputChannel :public MonitoringComponent {
	public:
		AnalogInputChannel(AnalogInConfiguration configuration, Ref<MonitoringComponent> parent = nullptr) 
			:MonitoringComponent(parent), configuration(configuration), _on_channel_trigger([](ChannelMessage){}),
			modbusAddress({configuration._register,RegisterType::Holding}) { 
			inputPin = ModuleAnalogInput(configuration.slot, configuration.channel);
		}

		void Initialize();

		bool isTriggered();

		AnalogInputChannel() :_on_channel_trigger([](ChannelMessage) {}) {	}

		void OnChannelTrigger(ChannelCallback cbk);

	private:
		ModuleAnalogInput inputPin;
		ModbusAddress modbusAddress;
		AnalogInConfiguration configuration;
		DistinctChannel channel;
		ChannelAlert alert1;
		ChannelAlert alert2;
		ChannelAlert alert3;

		float value;
		bool triggered;
		ChannelCallback _on_channel_trigger;


		void privateLoop();
	};
};


