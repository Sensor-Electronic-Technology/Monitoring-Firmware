#pragma once
#include <ArduinoSTL.h>
#include "Configuration.h"
#include "Data.h"

namespace MonitoringComponents {

	class ChannelAlert {
	public:
		int actionId;
		bool enabled;
		bool bypass;
		bool activated;
		ActionType actionType;

		ChannelAlert() {
			this->activated = false;
		}

		ChannelAlert& operator=(const ChannelAlert& rhs) {
			this->actionId = rhs.actionId;
			this->activated = rhs.activated;
			this->enabled = rhs.enabled;
			this->bypass = rhs.bypass;
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
		float setPointFactor;

		bool Check(float value) {
			return value >= this->setPoint;
		}

		AnalogAlert& operator=(const AnalogAlert& rhs) {
			ChannelAlert::operator=(rhs);
			this->setPoint = rhs.setPoint;
			this->setPointFactor = rhs.setPointFactor;
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