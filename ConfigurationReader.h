#pragma once
#include "ArduinoSTL.h"
#include <ArduinoJson.h>
#include <M2M_Logger.h>
#include <SD.h>
#include "AnalogInputChannel.h"
#include "DiscreteOuputChannel.h"
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

       std::vector<AnalogInputChannel> DeserializeAnalogConfig();
       std::vector<DiscreteOutputChannel> DeserializeOutputConfig();
       std::vector<DiscreteInputChannel> DeserializeDigitalConfig();
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

