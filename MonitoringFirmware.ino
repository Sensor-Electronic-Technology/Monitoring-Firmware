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

#define CREATEFILES 0
#define DEBUG 0

using namespace MonitoringComponents;

MonitoringController controller;

void(*resetFunc)(void) = 0;
bool resetLatched;

void setup(){
#if CREATEFILES==1
    Serial.begin(38400);
    while (!Serial) { ; }
    std::cout << "Starting Create Files" << std::endl;
    ConfigurationReader::CreatConfigFiles();
    while (1);
#else
    Serial.begin(115200);
#if DEBUG==1
    while (!Serial) { ; }
#endif
    if (!SD.begin(SDCARD_SS_PIN)) {
        while(1) {

            Serial.println(F("SD Card Initialize Failed!"));
        }
    }
    while (!P1.init()) { 
        Serial.println(F("Initializing Controller"));
        delay(100);
    }
    controller.Setup();
    controller.Initialize();
#endif
}

void loop(){
    controller.loop();
    ModbusService::Poll();
}
