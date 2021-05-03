#include "AnalogInputChannel.h"

namespace MonitoringComponents {
	
	void AnalogInputChannel::Initialize() {
		//DistinctChannel ch;
		//ch.slot = this->configuration.slot;
		//ch.channel = this->configuration.channel;
		//this->channel = ch;

		//this->alert1 = AnalogAlert(this->configuration.alert1,ch);
		//this->alert2 = ChannelAlert(this->configuration.alert2, ch);
		//this->alert3 = ChannelAlert(this->configuration.alert3, ch);

		//float reading = this->inputPin.read();
		//if (reading >= alert3.setPoint) {
		//	if (alert3.enabled) {
		//		ChannelMessage message;
		//		message. = ChannelAction::Trigger;
		//		message.channel = this->channel;
		//		alert3.activated = true;
		//		this->_on_channel_trigger(message);
		//	}

		//	if (alert2.activated) {
		//		ChannelMessage message;
		//		message.action = ChannelAction::Clear;
		//		message.channel = this->channel;
		//		alert2.activated = false;
		//		this->_on_channel_trigger(message);
		//	}

		//	if (alert1.activated) {
		//		ChannelMessage message;
		//		message.action = ChannelAction::Clear;
		//		message.channel = this->channel;
		//		alert2.activated = false;
		//		this->_on_channel_trigger(message);
		//	}
		//} else if (reading<alert3.setpoint && reading >= alert2.setpoint) {
		//	if (alert2.enabled) {

		//	}

		//	if (alert3.activated) {
		//		//clear
		//	}

		//	if (alert1.activated) {

		//	}

		//} else if (reading < alert2.setpoint && reading >= alert1.setpoint) {
		//	if (alert1.enabled) {

		//	}
		//	if (alert3.activated) {
		//		//clear
		//	}

		//	if (alert2.activated) {

		//	}
		//} else {
		//	if (alert1.activated) {

		//	}

		//	if (alert3.activated) {
		//		//clear
		//	}

		//	if (alert2.activated) {

		//	}
		//}
		//this->value = reading;
	}

	bool AnalogInputChannel::isTriggered() {
		//float value = this->inputPin.read();
		//float delta = value * .05;


		//for (int i = 0; i < MAXALERTS; i++) {
		//	float reading = this->inputPin.read();
		//	ChannelAlert& chAlert = this->channelAlerts[i];

		//	if (chAlert.alert.enabled && !chAlert.alert.bypass) {
		//		if (reading >= chAlert.alert.setpoint) {
		//			if (!chAlert.activated) {
		//				ChannelMessage message;
		//				message.action = ChannelAction::Trigger;
		//				message.channel = this->channel;
		//				chAlert.activated = true;
		//				this->_on_channel_trigger(message);
		//			}
		//		} else {
		//			
		//		}
		//	}//end check enabled
		//}
		return false;
	}
	
	
	void AnalogInputChannel::OnChannelTrigger(ChannelCallback cbk) {
		this->_on_channel_trigger = cbk;
	}

	void AnalogInputChannel::privateLoop() {

	}
};