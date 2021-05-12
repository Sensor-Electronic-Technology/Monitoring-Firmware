#pragma once
#include <ArduinoSTL.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include "Data.h"

class ModbusService{
public:
	ModbusService():ethServer(502) {
	}

	static ModbusService* const Instance() {
		if (instance == nullptr) {
			instance = new ModbusService;
		}
		return instance;
	}

	static bool Connected() {
		return true;
		//return ModbusService::Instance()->modbusServer.;
	}

	static void Initialize() {
		IPAddress ip(172, 20, 5,178);
		IPAddress subnet(255, 255, 0, 0);
		IPAddress dns(172, 20, 3,5);
		IPAddress gateway(172, 20, 5, 1);
		byte macAddress[6] = { 0x60, 0x52, 0xD0, 0x06, 0x70, 0x93 };
		Ethernet.init(5);   //CS pin for P1AM-ETH
		Ethernet.begin(macAddress);
		if (Ethernet.hardwareStatus() == EthernetNoHardware) {
			Serial.println("Ethernet shield not detected");
			//log error
		}
		if (Ethernet.linkStatus() == LinkOFF) {
			Serial.println("Ethernet cable is not connected");
		}
		auto instance = ModbusService::Instance();
		instance->ethServer.begin();
		IPAddress address=Ethernet.localIP();
		address.printTo(Serial);
		std::cout << std::endl;
		if (!instance->modbusServer.begin()) {
			std::cout << "Error starting Modbus Server" << std::endl;
		}
		instance->modbusServer.configureCoils(0, 1000);
		instance->modbusServer.configureDiscreteInputs(0, 121);
	}

	static void Poll() {
		auto instance = ModbusService::Instance();
		EthernetClient client = instance->ethServer.available();
		if (client) {
			instance->modbusServer.accept(client);
			while (client.connected()) {
				instance->modbusServer.poll();
			}
		}
	}

	static void UpdateHoldingRegister(int addr, uint16_t value) {
		auto instance = ModbusService::Instance();
		instance->modbusServer.holdingRegisterWrite(addr, value);
	}

	static void UpdateInputRegister(int addr, uint16_t value) {
		auto instance = ModbusService::Instance();
		instance->modbusServer.inputRegisterWrite(addr, value);
	}

	static void UpdateCoil(int addr, bool value) {
		auto instance = ModbusService::Instance();
		instance->modbusServer.coilWrite(addr, value);
	}

	static void UpdateDiscreteInput(int addr, bool value) {
		auto instance = ModbusService::Instance();
		instance->modbusServer.discreteInputWrite(addr, value);
	}

	//static void ReadHoldingRegister(int addr);
	//static void ReadInputRegister(int addr);
	//static void ReadCoil(int addr);
	//static void ReadDiscreteInput(int addr);

private:
	static ModbusService* instance;
	EthernetServer ethServer;
	ModbusTCPServer modbusServer;
};

