#include "DiscreteInputChannel.h"

namespace MonitoringComponents {
	bool DiscreteInputChannel::isTriggered() {
		if(this->connected){
			return (this->triggerOn==TriggerOn::Low) ? this->inputPin.isLow():this->inputPin.isHigh();
		}
		return false;

	}

	void DiscreteInputChannel::Initialize() {	
/* 		this->printTimer.onInterval([&](){
			switch(this->configuration.triggerOn){
				case TriggerOn::High:
					std::cout<<"D"<<this->modbusAddress.address<<" TriggerOn: "<<"High"<<" IsTriggered: "<<this->isTriggered()<<std::endl;
					break;
				case TriggerOn::Low:
					std::cout<<"D"<<this->modbusAddress.address<<" TriggerOn: "<<"Low"<<" IsTriggered: "<<this->isTriggered()<<std::endl;
					break;
				default:
					std::cout<<"D"<<this->modbusAddress.address<<" TriggerOn: "<<"Error"<<" IsTriggered: "<<this->isTriggered()<<std::endl;
					break;
			}
		 	
		 },1000);
	    RegisterChild(this->printTimer); */
		bool state = this->isTriggered();
		ModbusService::Update(this->modbusAddress,state);
		if(this->connected && this->alert.enabled){
			if (state) {
				ChannelMessage msg;
				msg.actionId = this->alert.actionId;
				msg.channel = this->inputPin.Address();
				msg.type = this->alert.actionType;
				msg.channelAction = ChannelAction::Trigger;
				ModbusService::Update(this->alertModAddress,uint16_t(this->alert.actionType));
				cout<<"Alert Mod Updated: Channel: "<<this->inputPin.Address().channel<<" Alert: "<<int(this->alert.actionType)<<endl;
				this->alert.activated = true;
				this->_on_state_change(msg);
			}else{
				ModbusService::Update(this->alertModAddress,uint16_t(ActionType::Okay));
			}
		}else{
			ModbusService::Update(this->modbusAddress,uint16_t(0));
			ModbusService::Update(this->alertModAddress,uint16_t(0));
		}
		this->triggered = state;
	}

	int DiscreteInputChannel::Channel() {
		return this->configuration.channel;
	}

	void DiscreteInputChannel::OnStateChange(ChannelCallback cbk) {
		this->_on_state_change = cbk;
	}

	void DiscreteInputChannel::privateLoop() {
		bool state = this->isTriggered();
		ModbusService::Update(this->modbusAddress,state);
		if (state!= this->triggered) {
			if (this->alert.enabled) {
				ChannelMessage message;
				message.actionId = this->alert.actionId;
				message.channel = this->inputPin.Address();
				message.type = this->alert.actionType;
				if (state) {
					message.channelAction = ChannelAction::Trigger;
					this->alert.activated = true;
					ModbusService::Update(this->alertModAddress,uint16_t(this->alert.actionType));
				} else {
					message.channelAction = ChannelAction::Clear;
					this->alert.activated = false;
					ModbusService::Update(this->alertModAddress,uint16_t(ActionType::Okay));
				}
				this->_on_state_change(message);
			}
			this->triggered = state;
		}
	}
};

