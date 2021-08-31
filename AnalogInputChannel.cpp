#include "AnalogInputChannel.h"

namespace MonitoringComponents {
	
	void AnalogInputChannel::Initialize() {
		float reading = this->inputPin.read();
		reading = reading * this->configuration.slope + this->configuration.offset;
		if (alert3.Check(reading) && alert3.enabled) {
			ChannelMessage message;
			message.actionId = alert3.actionId;
			message.channelAction = ChannelAction::Trigger;
			message.channel = inputPin.Address();
			message.type = alert3.actionType;
			alert3.activated = true;
			_on_channel_trigger(message);
		}else if(alert2.Check(reading) && alert2.enabled) {
			ChannelMessage message;
			message.actionId = alert2.actionId;
			message.channelAction = ChannelAction::Trigger;
			message.channel = inputPin.Address();
			message.type = alert2.actionType;
			alert2.activated = true;
			_on_channel_trigger(message);
		} else if (alert1.Check(reading) && alert1.enabled) {
			ChannelMessage message;
			message.actionId = alert1.actionId;
			message.channelAction = ChannelAction::Trigger;
			message.channel = inputPin.Address();
			message.type = alert1.actionType;
			alert1.activated = true;
			_on_channel_trigger(message);
		}
		this->value = value;
	}

	bool AnalogInputChannel::isTriggered() {
		return false;
	}
		
	void AnalogInputChannel::OnStateChange(ChannelCallback cbk) {
		this->_on_channel_trigger = cbk;
	}

	void AnalogInputChannel::privateLoop() {

		float reading = this->inputPin.read();
		reading = reading * this->configuration.slope + this->configuration.offset;
		ModbusService::UpdateInputRegister(this->configuration._register, reading*this->configuration.analogFactor);
		if (alert3.Check(reading) && alert3.enabled) {
			if (alert2) {
				alert2.activated = false;
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if (alert1) {
				alert1.activated = false;
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if (!alert3) {
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				alert3.activated = true;
				_on_channel_trigger(message);
			}
		} else if (alert2.Check(reading) && alert2.enabled) {
			if (alert3) {
				alert3.activated = false;
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if (alert1) {
				alert1.activated = false;
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if (!alert2) {
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				alert2.activated = true;
				_on_channel_trigger(message);
			}
		} else if (alert1.Check(reading) && alert1.enabled) {
			if (alert3) {
				alert3.activated = false;
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if (alert2) {
				alert2.activated = false;
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if (!alert1) {
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				alert1.activated = true;
				_on_channel_trigger(message);
			}
		} else {
			if (alert1 && alert1.enabled) {
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				alert1.activated = false;
				_on_channel_trigger(message);
			}

			if (alert2 && alert2.enabled) {
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				alert2.activated = false;
				_on_channel_trigger(message);
			}

			if (alert3 && alert3.enabled) {
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				alert3.activated = false;
				_on_channel_trigger(message);
			}
		}
	}
};