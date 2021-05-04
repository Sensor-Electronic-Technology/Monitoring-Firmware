#pragma once
#include <ArduinoSTL.h>
#include "Data.h"
#include "ChannelAlert.h"
#include "Ref.h"

namespace MonitoringComponents {

    class Configuration {
    public:
        int channel;
        ChannelAddress address;
        int _register;
        bool connected;

        Configuration() :channel(0), address({ 0, 0}), _register(0), connected(false) {   }

        Configuration(int channel,ChannelAddress address, int reg, bool connected)
            :channel(channel), address(address), _register(reg), connected(connected) {   }

        Configuration(const Configuration& other) {
            this->channel = other.channel;
            this->_register = other._register;
            this->connected = other.connected;
            this->address = other.address;
            this->channel = other.channel;
        }

        const Configuration& operator=(const Configuration& rhs) {
            this->channel = rhs.channel;
            this->_register = rhs._register;
            this->connected = rhs.connected;
            this->address = rhs.address;
            this->channel = rhs.channel;
            return *this;
        }
    };

    class DigitalInConfiguration :public Configuration {
    public:
        DigitalInConfiguration():Configuration(){  }

        DigitalInConfiguration(int channel,ChannelAddress addr, int reg, bool connected) :Configuration(channel,addr, reg, connected) {  }

        DigitalInConfiguration(const DigitalInConfiguration& other):Configuration(other) {
            this->triggerOn = other.triggerOn;
            this->alert = other.alert;
        }

        const DigitalInConfiguration& operator=(const DigitalInConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->triggerOn = rhs.triggerOn;
                this->alert = rhs.alert;
            }
            return *this;
        }

        TriggerOn triggerOn;
        DigitalAlert alert;
    };

    class AnalogInConfiguration :public Configuration {
    public:
        AnalogInConfiguration():Configuration(){}
        
        AnalogInConfiguration(int channel, ChannelAddress addr, int reg, bool connected) :Configuration(channel,addr, reg, connected) { }
        
        AnalogInConfiguration& operator=(const AnalogInConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->zeroValue = rhs.zeroValue;
                this->maxValue = rhs.maxValue;
                this->analogFactor = rhs.analogFactor;
                this->bypassAlerts = rhs.bypassAlerts;
                this->alert1 = rhs.alert1;
                this->alert2 = rhs.alert2;
                this->alert3 = rhs.alert3;
            }
            return *this;
        }

        float zeroValue;
        float maxValue;
        int analogFactor;
        AnalogAlert alert1;
        AnalogAlert alert2;
        AnalogAlert alert3;
        bool bypassAlerts;
    };

    class OutputConfiguration :public Configuration {
    public:
        OutputConfiguration() :Configuration() {}

        OutputConfiguration(int channel,ChannelAddress addr, int reg, State startState,bool connected) :Configuration(channel,addr, reg, connected),startState(startState) { }

        const OutputConfiguration& operator=(const OutputConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->startState = rhs.startState;
                this->triggerOn = rhs.triggerOn;
            }
            return *this;
        }

        State startState;
        TriggerOn triggerOn;
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

    class ActionConfiguration {
    public:
        int actionId;
        ActionType actionType;

        ChannelAddress addr1;
        State outputlevel1;

        ChannelAddress addr2;
        State outputlevel2;

        ChannelAddress addr3;
        State outputlevel3;

        State startState;
        OutputType type;
        ModbusAddress modbusAddress;

        const ActionConfiguration& operator=(const ActionConfiguration& rhs) {
            if (this != &rhs) {
                this->actionId = rhs.actionId;
                this->actionType = rhs.actionType;
                this->addr1 = rhs.addr1;
                this->outputlevel1 = rhs.outputlevel1;
                this->addr2 = rhs.addr2;
                this->outputlevel2= rhs.outputlevel2;
                this->addr3 = rhs.addr3;
                this->outputlevel3 = rhs.outputlevel3;

                this->startState = rhs.startState;
                this->type = rhs.type;
                this->modbusAddress = rhs.modbusAddress;
            }
            return *this;
        }
    };
};

