#pragma once

namespace MonitoringComponents {

	struct ChannelAddress {
		int slot;
		int channel;
		~ChannelAddress(){}

		bool operator==(ChannelAddress addr) {
			return this->channel == addr.channel && this->slot == addr.slot;
		}

		 operator bool() {
			 return this->channel != 0 && this->slot != 0;
		}
	};

	enum class State : bool {
		High = true,
		Low = false
	};

	enum class TriggerOn {
		High,
		Low
	};

	enum class RegisterType {
		Holding,
		Coil,
		DiscreteInput,
		Input
	};

	struct ModbusAddress {
		int address;
		RegisterType type;
	};

	enum class ChannelAction {
		Clear, Trigger
	};

	enum class ActionType:int {
		Okay=6,
		Alarm=5,
		Warning=4,
		SoftWarn=3,
		Maintenance=2,
		Custom=1
	};

	//enum class SystemState :int {
	//	Alarming,
	//	Warning,
	//	Okay
	//};



	enum InputType { Analog = 1, Digital = 0, NoType = 2 };
	enum LogicType { High = 1, Low = 0, NA = 2 };
	//enum AlertAction { Alarm = 4, Warn = 3, SoftWarn = 2, Maintenance = 1, Nothing = 0 };
	enum OutputAction { OutAlarm = 4, OutWarn = 3, Okay = 2, TriggerFromMap = 1, NoAction = 1 };
	enum OutputType { NO = 0, NC = 1, NOHC = 2, NCHO = 3 };
	enum ModuleType { DigitalInput = 1, DigitalOutput = 2, AnalogInput = 0, None = 3 };
	enum Module { P116ND3 = 0, P108ADL1 = 1, P108TD2 = 2, NoModule = 3 };
	enum ConfigurationType { DigitalIn, AnalogIn, Output, ModuleConfig, NoConfig };
	enum AlertPriority { Highest = 2, Medium = 1, Lowest = 0 };


	//struct Alert {
	//	float setpoint;
	//	int setPointFactor;
	//	AlertPriority prioirty;  //delete   
	//	ActionType action;
	//	bool bypass;
	//	bool enabled;

	//	Alert() {
	//		this->setpoint = 0;
	//		this->setPointFactor = 0;
	//		this->prioirty = AlertPriority::Lowest;
	//		this->enabled = false;
	//		this->bypass = false;
	//	}

	//	Alert(const Alert& alert) {
	//		this->setpoint = alert.setpoint;
	//		this->action = alert.action;
	//		this->enabled = alert.enabled;
	//		this->setPointFactor = alert.setPointFactor;
	//		this->bypass = alert.bypass;
	//		this->prioirty = alert.prioirty;
	//	}

	//	Alert& operator =(const Alert& rhs) {
	//		this->setpoint = rhs.setpoint;
	//		this->action = rhs.action;
	//		this->enabled = rhs.enabled;
	//		this->setPointFactor = rhs.setPointFactor;
	//		this->bypass = rhs.bypass;
	//		this->prioirty = rhs.prioirty;
	//		return *this;
	//	}
	//};
};