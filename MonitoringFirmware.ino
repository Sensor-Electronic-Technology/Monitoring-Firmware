
#include <M2M_Logger.h>
#include <SD.h>
#include <P1AM.h>
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
#include "DiscreteInputModule.h"

using namespace MonitoringComponents;

ConfigurationReader reader;
DiscreteInputModule discreteModule;



void setup(){
    Serial.begin(38400);
    while (!Serial) { ; }
    if (!SD.begin(SDCARD_SS_PIN)) {
        //cout<<"Error: Could not int SD card"<<endl;
        while (1);
    }
    Serial.println("SD Card Initialized, Initializing Reader");
    reader.Init();
    //cout << "Parsing and Configuring Module" << endl;
    Serial.println("Parsing and Configuring Module");
    vector<Ref<MonitoringComponent>> components;
    auto moduleConfigurations = reader.DeserializeModuleConfig();
    vector<DigitalInConfiguration> configurations=reader.DeserializeDigitalConfig();

    
    ModuleConfiguration config;
    config.moduleType = ModuleType::DigitalInput;
    config.channelCount = 8;
    config.slot = 1;
    discreteModule.SetConfig(config);
    discreteModule.CreateChannels(configurations);
    discreteModule.Print();


}

void loop(){
    discreteModule.loop();
    Serial.println("Loop: delaying for 200ms");
    delay(200);
}

void TestFileOpen() {

}

//void TestingConfiguration() {
//    Serial.begin(38400);
//    while (!Serial) { ; }
//    if (!SD.begin(SDCARD_SS_PIN)) {
//        cout << "Error: Could not int SD card" << endl;
//        while (1);
//    }
//    cout << "SD Card Initialized, Initializing Reader" << endl;
//    reader.Init();
//    cout << "Parsing Channels" << endl;
//    vector<DiscreteInputChannel> dChannels = reader.DeserializeDigitalConfig();
//    cout << "DiscreteInputs Parsed: " << " VectorSize: " << dChannels.size() << endl;
//    for (auto channel : dChannels) {
//        channel.Print();
//    }
//
//    auto oChannels = reader.DeserializeOutputConfig();
//    cout << "Outputs Parses: " << " VectorSize: " << oChannels.size() << endl;
//    for (auto channel : oChannels) {
//        channel.Print();
//    }
//}

void TestParseMapChannels() {
    Serial.begin(38400);
    while (!Serial) { ; }
    Serial.println("Creating Vector");
    vector<int> channelMaps;
    //while (!P1.init()) {};
    const char* mapto = "4,5,6,7,8";
    string str;
    string delimiter = ",";
    str = std::string(mapto);
    size_t pos = 0;
    string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        //cout << "Val: " << token << endl;
        int val = String(token.c_str()).toInt();
        //cout << "Int Val: " << val << endl;
        //channelMaps.push_back(std::strtol();
        //strtol()
        //strtol
        str.erase(0, pos + delimiter.length());
    }
}
