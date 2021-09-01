#include "AnalogInputChannel.h"

namespace MonitoringComponents {
	
	void AnalogInputChannel::Initialize() {

/* 		this->updateTimer.onInterval([&]() { 
				if(this->configuration.connected) {
					ModbusService::UpdateInputRegister(this->configuration._register, this->value);
					this->CheckProcessAlerts();
				} else {
					ModbusService::UpdateInputRegister(this->configuration._register, 0);
				}
			},UPDATEPERIOD);

		this->readTimer.onInterval([&]() { 
				this->Read();
			}, READPERIOD);
    	this->Read(); */
     
		if (alert3.Check(this->value) && alert3.enabled) {
			ChannelMessage message;
			message.actionId = alert3.actionId;
			message.channelAction = ChannelAction::Trigger;
			message.channel = inputPin.Address();
			message.type = alert3.actionType;
			alert3.activated = true;
			_on_channel_trigger(message);
		}else if(alert2.Check(this->value) && alert2.enabled) {
			ChannelMessage message;
			message.actionId = alert2.actionId;
			message.channelAction = ChannelAction::Trigger;
			message.channel = inputPin.Address();
			message.type = alert2.actionType;
			alert2.activated = true;
			_on_channel_trigger(message);
		} else if (alert1.Check(this->value) && alert1.enabled) {
			ChannelMessage message;
			message.actionId = alert1.actionId;
			message.channelAction = ChannelAction::Trigger;
			message.channel = inputPin.Address();
			message.type = alert1.actionType;
			alert1.activated = true;
			_on_channel_trigger(message);
		}
	}

	void AnalogInputChannel::OnStateChange(ChannelCallback cbk) {
		this->_on_channel_trigger = cbk;
	}

	void AnalogInputChannel::CheckProcessAlerts() {
		if(alert3.Check(this->value)) {
			if(alert2) {
				alert2.activated = false;
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if(alert1) {
				alert1.activated = false;
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if(!alert3) {
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				alert3.activated = true;
				_on_channel_trigger(message);
			}
		} else if(this->value<alert3.setPoint && alert2.Check(this->value)) {
			if(alert3) {
				alert3.activated = false;
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if(alert1) {
				alert1.activated = false;
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if(!alert2) {
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				alert2.activated = true;
				_on_channel_trigger(message);
			}
		} else if(this->value<alert2.setPoint && alert1.Check(this->value)) {
			if(alert3) {
				alert3.activated = false;
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if(alert2) {
				alert2.activated = false;
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				_on_channel_trigger(message);
			}
			if(!alert1) {
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				alert1.activated = true;
				_on_channel_trigger(message);
			}
		} else {
			if(alert1) {
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				alert1.activated = false;
				_on_channel_trigger(message);
			}

			if(alert2) {
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				alert2.activated = false;
				_on_channel_trigger(message);
			}

			if(alert3) {
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
	
	void AnalogInputChannel::Read() {
		float reading = this->inputPin.read();
		this->value+= ((reading * 62.5 -250)-this->value)*fWeight;
	}
	
	void AnalogInputChannel::privateLoop() {
		this->Read();
		ModbusService::UpdateInputRegister(this->configuration._register, this->value);
		this->CheckProcessAlerts();
	}
};
