
#include <SPI.h>
#include <Ethernet.h>
#include <M2M_Logger.h>
#include <SD.h>
#include <P1AM.h>
#include "ModbusService.h"
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <ArduinoSTL.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "AnalogInputChannel.h"
#include "MonitoringComponent.h"
#include "Configuration.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "ConfigurationReader.h"
#include "MonitoringController.h"

using namespace MonitoringComponents;

//ConfigurationReader reader;
//DiscreteInputModule discreteModule;
MonitoringController controller;

void setup(){
    Serial.begin(38400);
    while (!Serial) { ; }
    if (!SD.begin(SDCARD_SS_PIN)) {
        while (1);
    }
    while (!P1.init()) {;}
    ModbusService::Initialize();
    controller.Setup();
    controller.Initialize();

}

void loop(){
    controller.loop();
    ModbusService::Poll();
}
