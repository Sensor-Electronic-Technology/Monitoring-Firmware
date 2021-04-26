#pragma once
#include "Ref.h"
#include <ArduinoSTL.h>

namespace MonitoringComponents {

    enum InputType { Analog = 1, Digital = 0, NoType = 2 };
    enum LogicType { High = 1, Low = 0, NA = 2 };
    enum AlertAction { Alarm = 4, Warn = 3, SoftWarn = 2, Maintenance = 1, Nothing = 0 };
    enum OutputAction { OutAlarm = 4, OutWarn = 3, Okay = 2, TriggerFromMap = 1, NoAction = 1 };
    enum OutputType{NO=0,NC=1,NOHC=2, NCHO=3};
    enum ModuleType { DigitalInput = 1, DigitalOutput = 2, AnalogInput = 0, None = 3 };
    enum Module { P116ND3 = 0, P108ADL1 = 1, P108TD2 = 2, NoModule = 3 };
    enum ConfigurationType { DigitalIn, AnalogIn, Output, ModuleConfig, NoConfig };
    enum AlertPriority{Highest=2,Medium=1,Lowest=0};

    struct Alert {
        float setpoint;
        int setPointFactor;
        AlertPriority prioirty;     
        AlertAction action;
        bool bypass;
        bool enabled;

        Alert() {
            this->setpoint = 0;
            this->setPointFactor = 0;
            this->action = AlertAction::Nothing;
            this->prioirty = AlertPriority::Lowest;
            this->enabled = false;
            this->bypass = false;
        }

        Alert(const Alert& alert) {
            this->setpoint = alert.setpoint;
            this->action = alert.action;
            this->enabled = alert.enabled;
            this->setPointFactor = alert.setPointFactor;
            this->bypass = alert.bypass;
            this->prioirty = alert.prioirty;
        }

        Alert& operator =(const Alert& rhs) {
            this->setpoint = rhs.setpoint;
            this->action = rhs.action;
            this->enabled = rhs.enabled;
            this->setPointFactor = rhs.setPointFactor;
            this->bypass = rhs.bypass;
            this->prioirty = rhs.prioirty;
            return *this;
        }
    };

    class Configuration {
    public:
        int channel;
        int slot;
        int _register;
        bool connected;

        Configuration():channel(0),slot(0),_register(0),connected(false) {   }

        Configuration(int channel, int slot, int reg, bool connected)
            :channel(channel), slot(slot), _register(reg), connected(connected) {   }

        Configuration(const Configuration& other) {
            this->channel = other.channel;
            this->slot = other.slot;
            this->_register = other._register;
            this->connected = other.connected;
        }

        const Configuration& operator=(const Configuration& rhs) {
            this->channel = rhs.channel;
            this->_register = rhs._register;
            this->connected = rhs.connected;
            this->slot = rhs.slot;
            return *this;
        }
    };

    class DigitalInConfiguration :public Configuration {
    public:
        DigitalInConfiguration():Configuration(){  }

        DigitalInConfiguration(int channel, int slot, int reg, bool connected):Configuration(channel,slot,reg,connected) {  }

        DigitalInConfiguration(const DigitalInConfiguration& other):Configuration(other) {
            this->Logic = other.Logic;
            this->alert = other.alert;
        }

        const DigitalInConfiguration& operator=(const DigitalInConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->Logic = rhs.Logic;
                this->alert = rhs.alert;
            }
            return *this;
        }

        LogicType Logic;
        Alert alert;
    };

    class AnalogInConfiguration :public Configuration {
    public:
        AnalogInConfiguration():Configuration(){}
        
        AnalogInConfiguration(int channel, int slot, int reg, bool connected) :Configuration(channel, slot, reg, connected) { }
        
        AnalogInConfiguration& operator=(const AnalogInConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->zeroValue = rhs.zeroValue;
                this->maxValue = rhs.maxValue;
                this->analogFactor = rhs.zeroValue;
                this->bypassAlerts = rhs.bypassAlerts;
                this->alerts = rhs.alerts;
            }
            return *this;
        }

        float zeroValue;
        float maxValue;
        int analogFactor;
        std::vector<Alert> alerts;
        bool bypassAlerts;
    };

    class OutputConfiguration :public Configuration {
    public:
        OutputConfiguration() :Configuration() {}

        OutputConfiguration(int channel, int slot, int reg, bool connected) :Configuration(channel, slot, reg, connected) { }

        const OutputConfiguration& operator=(const OutputConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->action = rhs.action;
                this->outputType = rhs.outputType;
                this->startState = rhs.startState;
                this->channelMaps = rhs.channelMaps;
            }
            return *this;
        }

        std::vector<int> channelMaps;
        LogicType startState;
        OutputAction action;
        OutputType outputType;
        LogicType triggerOn;
    };

    class ModuleConfiguration {
    public:
        Module moduleName;
        ModuleType moduleType;
        int slot;
        int channelCount;
        int channelMapEnd;

        const ModuleConfiguration& operator=(const ModuleConfiguration& rhs) {
            if (this != &rhs) {
                //Configuration::operator=(rhs);
                this->moduleName = rhs.moduleName;
                this->moduleType = rhs.moduleType;
                this->slot = rhs.slot;
                this->channelCount = rhs.channelCount;
                this->channelMapEnd = rhs.channelMapEnd;
            }
            return *this;
        }

        String ToString() {
            String output = "";
            output += "Module: " + String((int)this->moduleName);
            output += "Type: " + String((int)this->moduleType);
            output += "Slot: " + String(this->slot);
            output += "ChannelCount: " + String(this->channelCount);
            output += "ChannelMapEnd: " + String(this->channelMapEnd);
            return output;
        }

    };
};

