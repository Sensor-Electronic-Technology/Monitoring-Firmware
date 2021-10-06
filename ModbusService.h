#pragma once
#include <ArduinoSTL.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include "Configuration.h"
#include "Data.h"

namespace MonitoringComponents {
	class ModbusService {
	public:
		ModbusService() :ethServer(502) { }

		static ModbusService* const Instance() {
			if (instance == nullptr) {
				instance = new ModbusService;
			}
			return instance;
		}

		static bool Connected() {
			return true;
		}

		static void Initialize(NetConfiguration netConfig) {
			//IPAddress dns(172, 20, 3, 5);
			//IPAddress gateway(172, 20, 5, 1);

			//byte macAddress[6] = { 0x60, 0x52, 0xD0, 0x06, 0x70, 0x59 };
			IPAddress subnet(255, 255, 255, 0);		
			auto instance = ModbusService::Instance();
			instance->config = netConfig;
			Ethernet.init(5);   //CS pin for P1AM-ET
			Ethernet.setDnsServerIP(instance->config.dns);
			Ethernet.setGatewayIP(instance->config.gateway);
			Ethernet.setSubnetMask(subnet);
			if (Ethernet.hardwareStatus() == EthernetHardwareStatus::EthernetNoHardware || Ethernet.linkStatus() == EthernetLinkStatus::LinkOFF) {
				instance->initialized = false;
				std::cout << "Ethernet hardware or Ethernet Link not available" << std::endl;
				return;
			}
			if (Ethernet.begin(instance->config.mac,10000UL,4000UL)) {
				instance->ethServer.begin();
				IPAddress address = Ethernet.localIP();
				address.printTo(Serial);
				std::cout << std::endl;
				if (instance->modbusServer.begin()) {
					instance->initialized = true;
					instance->modbusServer.configureCoils(0, instance->config.coils);
					instance->modbusServer.configureInputRegisters(0, instance->config.inputRegisters);
					instance->modbusServer.configureDiscreteInputs(0, instance->config.discreteInputs);
				} else {
					instance->initialized = false;
					std::cout << "ModbusServer Failed To Initialize" << std::endl;
				}
			} else {
				instance->initialized = false;
				std::cout << "Ethernet failed to initialize" << std::endl;
			}
		}

		static void Poll() {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				EthernetClient client = instance->ethServer.available();
				if (client) {
					instance->modbusServer.accept(client);
					while (client.connected()) {
						instance->modbusServer.poll();
					}
				}
				Ethernet.maintain();
			}
		}

		static void UpdateHoldingRegister(int addr, uint16_t value) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				instance->modbusServer.holdingRegisterWrite(addr, value);
			}
			
		}

		static void UpdateInputRegister(int addr, uint16_t value) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				instance->modbusServer.inputRegisterWrite(addr, value);
			}
			
		}

		static void UpdateCoil(int addr, bool value) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				instance->modbusServer.coilWrite(addr, value);
			}
			
		}

		static void UpdateDiscreteInput(int addr, bool value) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				instance->modbusServer.discreteInputWrite(addr, value);
			}
			
		}

		static bool ReadCoil(int addr) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				return instance->modbusServer.coilRead(addr);
			} else {
				return false;
			}
		}

		static int ReadDiscreteInputRegister(int addr) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				return instance->modbusServer.discreteInputRead(addr);
			} else {
				return 0;
			}
			
		}

		static long ReadHoldingRegister(int addr) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				return instance->modbusServer.holdingRegisterRead(addr);
			} else {
				return 0;
			}
			
		}

		static long ReadInputRegister(int addr) {
			auto instance = ModbusService::Instance();
			if (instance->initialized) {
				return instance->modbusServer.inputRegisterRead(addr);
			}else {
				return 0;
			}
			
		}

	private:
		static ModbusService* instance;
		EthernetServer ethServer;
		ModbusTCPServer modbusServer;
		bool initialized = false;
		NetConfiguration config;
	};
};



