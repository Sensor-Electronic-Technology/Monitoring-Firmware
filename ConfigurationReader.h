#pragma once
#include "ArduinoSTL.h"
#include <ArduinoJson.h>
#include <M2M_Logger.h>
#include <SD.h>
#include "AnalogInputChannel.h"
#include "DiscreteOutputChannel.h"
#include "DiscreteInputChannel.h"



namespace MonitoringComponents {

    class ConfigurationReader {
    public:
        ConfigurationReader();
        ConfigurationReader(Logger* logger);
        ~ConfigurationReader();
        bool Init();
        ConfigurationReader& operator=(const ConfigurationReader& rhs) {
            this->configFile = rhs.configFile;
            this->logger = rhs.logger;
            this->configLoaded = rhs.configLoaded;
            return *this;
        };
        void GetFileSizes();
        void SetSize(int lineCount, int value);
        void PrintSizes();

       std::vector<AnalogInConfiguration> DeserializeAnalogConfig();
       std::vector<OutputConfiguration> DeserializeOutputConfig();
       std::vector<DigitalInConfiguration> DeserializeDigitalConfig();
       std::vector<ModuleConfiguration> DeserializeModuleConfig();

    private:
        long DigitalInSize;
        long AnalogInSize;
        long ModuleSize;
        long OutputSize;
        File configFile;
        Logger* logger = nullptr;
        bool configLoaded = false;
    };
};

