#pragma once
#include <ArduinoSTL.h>
#include "Configuration.h"
#include "Data.h"

#define  AlertDeviation 2

namespace MonitoringComponents {

	class ChannelAlert {
	public:
		int actionId;
		bool enabled;
		bool activated;
		ActionType actionType;

		ChannelAlert() {
			this->activated = false;
		}

		ChannelAlert& operator=(const ChannelAlert& rhs) {
			this->actionId = rhs.actionId;
			this->activated = rhs.activated;
			this->enabled = rhs.enabled;
			this->actionType = rhs.actionType;
			return *this;
		}

		operator bool() {
			return this->activated;
		}
	};

	class AnalogAlert :public ChannelAlert {
	public:
		float setPoint;

		bool Check(float value) {
			if(this->enabled){
				return value >= (this->setPoint-(this->setPoint*0.002f));	
			}else{
				return false;
			}
		}

		AnalogAlert& operator=(const AnalogAlert& rhs) {
			ChannelAlert::operator=(rhs);
			this->setPoint = rhs.setPoint;
			return *this;
		}
	};

	class DigitalAlert :public ChannelAlert {
	public:
		TriggerOn triggerOn;
		DigitalAlert& operator=(const DigitalAlert& rhs) {
			ChannelAlert::operator=(rhs);
			this->triggerOn = rhs.triggerOn;
		}
	};
};