#include "AnalogInputChannel.h"

namespace MonitoringComponents {
	
	void AnalogInputChannel::Initialize() {

 		this->updateTimer.onInterval([&]() { 
				if(this->configuration.connected){
					float value=this->Read();
					ModbusService::Update(this->modbusAddress, value*this->analogFactor);
					this->CheckProcessAlerts(value);
				}else{
					ModbusService::Update(this->modbusAddress,uint16_t(0));
					ModbusService::Update(this->alertModAddress,uint16_t(0));
				}
			},UPDATEPERIOD);
		RegisterChild(this->updateTimer);
    	float value=this->Read();
		if(!this->configuration.connected){
			ModbusService::Update(this->modbusAddress,uint16_t(0));
			ModbusService::Update(this->alertModAddress,uint16_t(0));
		}else{
			if (alert3.Check(value) && alert3.enabled) {
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				alert3.activated = true;
				ModbusService::Update(this->alertModAddress,uint16_t(alert3.actionType));
				_on_channel_trigger(message);
			}else if(alert2.Check(value) && alert2.enabled) {
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				alert2.activated = true;
				ModbusService::Update(this->alertModAddress,uint16_t(alert2.actionType));
				_on_channel_trigger(message);
			} else if (alert1.Check(value) && alert1.enabled) {
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				alert1.activated = true;
				ModbusService::Update(this->alertModAddress,uint16_t(alert1.actionType));
				_on_channel_trigger(message);
			}
		}
	}

	void AnalogInputChannel::OnStateChange(ChannelCallback cbk) {
		this->_on_channel_trigger = cbk;
	}

	void AnalogInputChannel::CheckProcessAlerts(float value) {
		if(alert3.Check(value)) {
			if(alert2) {
				alert2.activated = false;
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				_on_channel_trigger(message);
			}
			if(alert1) {
				alert1.activated = false;
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				_on_channel_trigger(message);
			}
			if(!alert3) {
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				alert3.activated = true;
				ModbusService::Update(this->alertModAddress,uint16_t(alert3.actionType));
				_on_channel_trigger(message);
			}
		} else if(value<alert3.setPoint && alert2.Check(value)) {
			if(alert3) {
				alert3.activated = false;
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				_on_channel_trigger(message);
			}
			if(alert1) {
				alert1.activated = false;
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				_on_channel_trigger(message);
			}
			if(!alert2) {
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				alert2.activated = true;
				ModbusService::Update(this->alertModAddress,uint16_t(alert2.actionType));
				_on_channel_trigger(message);
			}
		} else if(value<alert2.setPoint && alert1.Check(value)) {
			if(alert3) {
				alert3.activated = false;
				ChannelMessage message;
				message.actionId = alert3.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert3.actionType;
				_on_channel_trigger(message);
			}
			if(alert2) {
				alert2.activated = false;
				ChannelMessage message;
				message.actionId = alert2.actionId;
				message.channelAction = ChannelAction::Clear;
				message.channel = inputPin.Address();
				message.type = alert2.actionType;
				_on_channel_trigger(message);
			}
			if(!alert1) {
				ChannelMessage message;
				message.actionId = alert1.actionId;
				message.channelAction = ChannelAction::Trigger;
				message.channel = inputPin.Address();
				message.type = alert1.actionType;
				alert1.activated = true;
				ModbusService::Update(this->alertModAddress,uint16_t(alert1.actionType));
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
			ModbusService::Update(this->alertModAddress,uint16_t(ActionType::Okay));
		}
	}
	
	float AnalogInputChannel::Read() {
		/*float reading = this->inputPin.read();
		if(reading>=4.00f){
			this->value += ((reading * this->configuration.slope + this->configuration.offset)-this->value)*fWeight;
		}else{
			this->value += ((4.00f * this->configuration.slope + this->configuration.offset)-this->value)*fWeight;
		}*/		
		float value=0.00f;
		float reading=this->inputPin.read();
		std::cout<<"A"<<this->modbusAddress.address<<": "<<reading<<std::endl;
		value=reading*this->configuration.slope+this->configuration.offset;	
		return value;
	}
	
	void AnalogInputChannel::privateLoop() {

	}
};
