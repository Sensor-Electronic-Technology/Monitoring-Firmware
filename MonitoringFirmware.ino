
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
#include <string>
#include <iostream>


void setup()
{
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
        cout<<"Val: "<< token << endl;
        //channelMaps.push_back(std::strtol();
        //strtol()
        //strtol
        str.erase(0, pos + delimiter.length());
    }
}

// Add the main program code into the continuous loop() function
void loop()
{


}
