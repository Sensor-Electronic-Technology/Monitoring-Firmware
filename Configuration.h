#pragma once
#include <ArduinoSTL.h>
#include "Data.h"
#include "ChannelAlert.h"
#include <Ethernet.h>
#include "Ref.h"


namespace MonitoringComponents {

    class Configuration {
    public:
        int channel;
        ChannelAddress address;
        ModbusAddress _modbusAddress;
        bool connected;

        Configuration() :channel(0), address({ 0, 0}), connected(false) {   }

        Configuration(int channel,ChannelAddress address, ModbusAddress modbusAddress, bool connected)
            :channel(channel), address(address), _modbusAddress(modbusAddress), connected(connected) {   }

        Configuration(const Configuration& other) {
            this->channel = other.channel;
            this->address=other.address;
            this->_modbusAddress=other._modbusAddress;
            this->connected = other.connected;
            this->address = other.address;
            this->channel = other.channel;
        }

        const Configuration& operator=(const Configuration& rhs) {
            this->channel = rhs.channel;
            this->address=rhs.address;
            this->_modbusAddress=rhs._modbusAddress;
            this->connected = rhs.connected;
            this->address = rhs.address;
            this->channel = rhs.channel;
            return *this;
        }
    };

    class DigitalInConfiguration :public Configuration {
    public:
        DigitalInConfiguration():Configuration(){  }

        DigitalInConfiguration(int channel,ChannelAddress addr,ModbusAddress inAddress,ModbusAddress aAddress, bool connected) 
        :Configuration(channel,addr, inAddress, connected) {  
            this->alertAddress=aAddress;
        }

        DigitalInConfiguration(const DigitalInConfiguration& other):Configuration(other) {
            this->alertAddress=other.alertAddress;
            this->triggerOn = other.triggerOn;
            this->alert = other.alert;
        }

        const DigitalInConfiguration& operator=(const DigitalInConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->alertAddress=rhs.alertAddress;
                this->triggerOn = rhs.triggerOn;
                this->alert = rhs.alert;
            }
            return *this;
        }
        ModbusAddress alertAddress;
        TriggerOn triggerOn;
        DigitalAlert alert;
    };

    class VirtualDigitalConfiguration {
    public:
        VirtualDigitalConfiguration():enabled(false){ }

        VirtualDigitalConfiguration(int in,ModbusAddress inAddress,ModbusAddress aAddress,bool en)
            :input(in), modbusAddress(inAddress),alertAddress(aAddress), enabled(en) { }

        VirtualDigitalConfiguration(const VirtualDigitalConfiguration& config) {
            this->input = config.input;
            this->modbusAddress=config.modbusAddress;
            this->alertAddress=config.alertAddress;
            this->enabled = config.enabled;
            this->alert = config.alert;
            this->triggerOn = config.triggerOn;
        }

        const VirtualDigitalConfiguration& operator=(const VirtualDigitalConfiguration& rhs) {
            this->input = rhs.input;
            this->modbusAddress=rhs.modbusAddress;
            this->alertAddress=rhs.alertAddress;
            this->enabled = rhs.enabled;
            this->alert = rhs.alert;
            this->triggerOn = rhs.triggerOn;
            return *this;
        }

        int input;
        ModbusAddress modbusAddress;
        ModbusAddress alertAddress;
        bool enabled;
        DigitalAlert alert;
        TriggerOn triggerOn;
    };

    class AnalogInConfiguration :public Configuration {
    public:
        AnalogInConfiguration():Configuration(){}
        
        AnalogInConfiguration(int channel, ChannelAddress addr, ModbusAddress inAddress,ModbusAddress aAddress, bool connected) 
        : Configuration(channel,addr, inAddress, connected),alertAddress(aAddress) { }

        AnalogInConfiguration(const AnalogInConfiguration& other):Configuration(other){
            this->slope=other.slope;
            this->offset=other.offset;
            this->analogFactor=other.analogFactor;
            this->alertAddress=other.alertAddress;
            this->alert1=other.alert1;
            this->alert2=other.alert2;
            this->alert3=other.alert3;
        }

        AnalogInConfiguration& operator=(const AnalogInConfiguration& rhs) {
            if (this != &rhs) {
                Configuration::operator=(rhs);
                this->alertAddress=rhs.alertAddress;
                this->slope = rhs.slope;
                this->offset = rhs.offset;
                this->analogFactor = rhs.analogFactor;
                this->bypassAlerts = rhs.bypassAlerts;
                this->alert1 = rhs.alert1;
                this->alert2 = rhs.alert2;
                this->alert3 = rhs.alert3;
            }
            return *this;
        }

        float slope;
        float offset;
        int analogFactor;
        ModbusAddress channelAddress;
        ModbusAddress alertAddress;
        AnalogAlert alert1;
        AnalogAlert alert2;
        AnalogAlert alert3;
        bool bypassAlerts;
    };

    class OutputConfiguration :public Configuration {
    public:
        OutputConfiguration() :Configuration() {}

        OutputConfiguration(int channel,ChannelAddress addr, ModbusAddress inAddress, State startState,bool connected) 
        :Configuration(channel,addr,inAddress, connected),startState(startState) { }

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
        const char* moduleName;
        ModuleType moduleType;
        int slot;
        int channelCount;
        int channelMapEnd;
        ModbusAddress modbusRegister;

        const ModuleConfiguration& operator=(const ModuleConfiguration& rhs) {
            if (this != &rhs) {
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
            output += "Module: " + String(this->moduleName);
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
        State onLevel1;
        State offLevel1;

        ChannelAddress addr2;
        State onLevel2;
        State offLevel2;

        ChannelAddress addr3;
        State onLevel3;
        State offLevel3;

        State startState;
        OutputType type;
        ModbusAddress modbusAddress;

        const ActionConfiguration& operator=(const ActionConfiguration& rhs) {
            if (this != &rhs) {
                this->actionId = rhs.actionId;
                this->actionType = rhs.actionType;
                this->addr1 = rhs.addr1;
                this->onLevel1 = rhs.onLevel1;
                this->offLevel1 = rhs.offLevel1;
                this->addr2 = rhs.addr2;
                this->onLevel2= rhs.onLevel2;
                this->offLevel2 = rhs.offLevel2;
                this->addr3 = rhs.addr3;
                this->onLevel3 = rhs.onLevel3;
                this->offLevel3 = rhs.offLevel3;
                this->startState = rhs.startState;
                this->type = rhs.type;
                this->modbusAddress = rhs.modbusAddress;
            }
            return *this;
        }
    };

    class NetConfiguration {
    public:
        int inputRegisters;
        int holdingRegisters;
        int discreteInputs;
        int coils;
        ModbusAddress modbusAddress;
        byte mac[6];
        IPAddress dns;
        IPAddress gateway;
        IPAddress ip;

        const NetConfiguration& operator=(const NetConfiguration& rhs) {
            this->inputRegisters = rhs.inputRegisters;
            this->holdingRegisters=rhs.holdingRegisters;
            this->modbusAddress=rhs.modbusAddress;
            this->coils = rhs.coils;
            this->discreteInputs = rhs.discreteInputs;
            this->ip=rhs.ip;
            std::copy(rhs.mac, rhs.mac + 6, this->mac);
            this->dns = rhs.dns;
            this->gateway = rhs.gateway;
        }
    };

};

