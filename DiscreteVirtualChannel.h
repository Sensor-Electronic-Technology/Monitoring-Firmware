#pragma once
#include <ArduinoSTL.h>
#include "ModbusService.h"
#include "Function.h"
#include "IO.h"
#include "Callbacks.h"
#include "MonitoringComponent.h"
#include "Configuration.h"

namespace MonitoringComponents {
	class DiscreteVirtualChannel : public MonitoringComponent {
	public:

		DiscreteVirtualChannel(VirtualDigitalConfiguration config, Ref<MonitoringComponent> parent = nullptr)
			:MonitoringComponent(parent), configuration(config), modbusAddress({ config._register,RegisterType::Coil }),
			_on_state_change([](ChannelMessage) {}) {
			this->triggerOn = config.triggerOn;
			this->enabled = config.enabled;
			this->alert = this->configuration.alert;
		}

		DiscreteVirtualChannel():MonitoringComponent(nullptr), _on_state_change([](ChannelMessage){	}) {	};

		void Initialize() {
			bool state = this->isTriggered();
			if (state) {
				ChannelMessage message;
				message.actionId = this->alert.actionId;
				message.channel = ChannelAddress();
				message.type = this->alert.actionType;
				message.channelAction = ChannelAction::Trigger;

				this->alert.activated = true;
				this->_on_state_change(message);
			}
			this->triggered = state;
		}

		bool isTriggered() {
			if (this->enabled) {
				bool value = ModbusService::ReadCoil(this->modbusAddress.address);
				switch (this->triggerOn) {
					case TriggerOn::High: {
						return value;
					}
					case TriggerOn::Low: {
						return !value;
					}
					default: {
						return false; 
					}
				}
			}
			return false;
		}

		void OnStateChange(ChannelCallback cbk) {
			this->_on_state_change = cbk;
		}

	private:
		VirtualDigitalConfiguration configuration;
		TriggerOn triggerOn;
		ModbusAddress modbusAddress;
		DigitalAlert alert;
		bool triggered;
		bool enabled;
		ChannelCallback _on_state_change;
		
		void privateLoop() {
			bool state = this->isTriggered();
			if (state != this->triggered) {
				if (this->alert.enabled) {
					ChannelMessage  message;
					message.actionId = this->alert.actionId;
					message.channel = ChannelAddress();
					message.type = this->alert.actionType;
					if (state) {
						message.channelAction = ChannelAction::Trigger;
						this->alert.activated = true;
					}else {
						message.channelAction = ChannelAction::Clear;
						this->alert.activated = false;
					}
					this->_on_state_change(message);
				}
				this->triggered = state;
			}
		}
	};
}


