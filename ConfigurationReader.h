#pragma once
#include <ArduinoSTL.h>
#include <ArduinoJson.h>
#include <M2M_Logger.h>
#include <SD.h>
#include "AnalogInputChannel.h"
#include "DiscreteOutputChannel.h"
#include "DiscreteInputChannel.h"
#include "DiscreteVirtualChannel.h"
#include "Configuration.h"

namespace MonitoringComponents {

    class ConfigurationReader {
    public:
        ConfigurationReader();
        ~ConfigurationReader();
        bool Init();
        ConfigurationReader& operator=(const ConfigurationReader& rhs) {
            this->configFile = rhs.configFile;
            this->configLoaded = rhs.configLoaded;
            return *this;
        };
        void GetFileSizes();
        
        void SetSize(int lineCount, int value);
        void PrintSizes();

        static void CreatConfigFiles();
        static char nibbleTobyte(char c);
        static char ToHex(char c1, char c2);
        static std::string HexToString(char* data);

       std::vector<AnalogInConfiguration> DeserializeAnalogConfig();
       std::vector<OutputConfiguration> DeserializeOutputConfig();
       std::vector<DigitalInConfiguration> DeserializeDigitalConfig();
       std::vector<ActionConfiguration> DeserializeActions();
       std::vector<VirtualDigitalConfiguration> DeserializeVirtualConfig();
       NetConfiguration DeserializeNetConfiguration();

    private:
        long DigitalInSize;
        long AnalogInSize;
        long ModuleSize;
        long OutputSize;
        long ActionSize;
        long NetConfigSize;
        long VirtualSize;
        File configFile;
        bool configLoaded = false;
    };
};

