#pragma once

namespace MonitoringComponents {

	struct ChannelAddress {
		int slot;
		int channel;

		ChannelAddress() {
			this->slot = 0;
			this->channel = 0;
		}

		ChannelAddress(int slot, int channel) {
			this->slot = slot;
			this->channel = channel;
		}

		~ChannelAddress(){}

		ChannelAddress& operator=(const ChannelAddress& rhs) {
			this->channel = rhs.channel;
			this->slot = rhs.slot;
			return *this;
		}

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
		Holding=1,
		Coil=2,
		DiscreteInput=3,
		Input=4
	};

	struct ModbusAddress {
	public:
		int address;
		RegisterType type;

		ModbusAddress(){
			this->address=0;
			this->type=RegisterType::DiscreteInput;
		}

		ModbusAddress(const ModbusAddress& other){
			this->address=other.address;
			this->type=other.type;
		}

		ModbusAddress& operator=(const ModbusAddress& rhs){
			this->address=rhs.address;
			this->type=rhs.type;
			return *this;
		}
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

	enum InputType { Analog = 1, Digital = 0, NoType = 2 };
	enum LogicType { High = 1, Low = 0, NA = 2 };
	enum OutputAction { OutAlarm = 4, OutWarn = 3, Okay = 2, TriggerFromMap = 1, NoAction = 1 };
	enum OutputType { NO = 0, NC = 1, NOHC = 2, NCHO = 3 };
	enum ModuleType { DigitalInput = 1, DigitalOutput = 2, AnalogInput = 0, None = 3 };
	enum Module { P116ND3 = 0, P108ADL1 = 1, P108TD2 = 2, NoModule = 3 };
	enum ConfigurationType { DigitalIn, AnalogIn, Output, ModuleConfig, NoConfig };
	enum AlertPriority { Highest = 2, Medium = 1, Lowest = 0 };
};