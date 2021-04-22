
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
#include "Module.h"

using namespace MonitoringComponents;

ConfigurationReader reader;

void setup(){
    Serial.begin(38400);
    while (!Serial) { ; }
    if (!SD.begin(SDCARD_SS_PIN)) {
        cout<<"Error: Could not int SD card"<<endl;
        while (1);
    }
    cout << "SD Card Initialized, Initializing Reader" << endl;
    reader.Init();
    cout << "Parsing Channels" << endl;
    vector<Ref<MonitoringComponent>> components;
    auto moduleConfigurations = reader.DeserializeModuleConfig();
    vector<DiscreteInputChannel> dChannels = reader.DeserializeDigitalConfig();
    cout << "dChannel.size() " << dChannels.size() << endl;
    for (auto channel : dChannels) {
        components.push_back(Ref<MonitoringComponent>(channel));
    }
    cout << " Components.size() " << components.size()<<endl;
    for (auto c : components) {
        c->Print();
        cout << "Here" << endl;
    }
    cout << "after" << endl;
}

// Add the main program code into the continuous loop() function
void loop(){


}

void TestFileOpen() {

}

void TestingConfiguration() {
    Serial.begin(38400);
    while (!Serial) { ; }
    if (!SD.begin(SDCARD_SS_PIN)) {
        cout << "Error: Could not int SD card" << endl;
        while (1);
    }
    cout << "SD Card Initialized, Initializing Reader" << endl;
    reader.Init();
    cout << "Parsing Channels" << endl;
    vector<DiscreteInputChannel> dChannels = reader.DeserializeDigitalConfig();
    cout << "DiscreteInputs Parsed: " << " VectorSize: " << dChannels.size() << endl;
    for (auto channel : dChannels) {
        channel.Print();
    }

    auto oChannels = reader.DeserializeOutputConfig();
    cout << "Outputs Parses: " << " VectorSize: " << oChannels.size() << endl;
    for (auto channel : oChannels) {
        channel.Print();
    }
}

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
        cout << "Val: " << token << endl;
        int val = String(token.c_str()).toInt();
        cout << "Int Val: " << val << endl;
        //channelMaps.push_back(std::strtol();
        //strtol()
        //strtol
        str.erase(0, pos + delimiter.length());
    }
}
