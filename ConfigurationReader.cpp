#include "ConfigurationReader.h"

#define AnalogFile              "analog.txt"
#define OutputFile              "output.txt"
#define DigitalFile             "digital.txt"
#define ModuleFile              "module.txt"
#define SizeFile                "size.txt"
#define LOG_FILENAME            "log.txt"

namespace MonitoringComponents {

    ConfigurationReader::ConfigurationReader(Logger* logger) {
        this->logger = logger;
        this->configLoaded = false;
        this->DigitalInSize = 0;
        this->OutputSize = 0;
        this->AnalogInSize = 0;
        this->ModuleSize = 0;
    }

    ConfigurationReader::ConfigurationReader() {
        this->configLoaded = false;
        this->logger = nullptr;
        this->DigitalInSize = 0;
        this->OutputSize = 0;
        this->AnalogInSize = 0;
        this->ModuleSize = 0;
    }

    void ConfigurationReader::PrintSizes() {
        //std::cout << AnalogFile << " " << this->AnalogInSize << std::endl;
        //std::cout << OutputFile << " " << this->OutputSize << std::endl;
        //std::cout << DigitalFile << " " << this->DigitalInSize << std::endl;
        //std::cout << ModuleFile << " " << this->ModuleSize << std::endl;
    }

    void ConfigurationReader::GetFileSizes() {
        std::cout << "Opening File" << std::endl;
        File file = SD.open("size.txt");
        String value = "";
        int lineCount = 0;
        if (file) {
            std::cout << "File Opened" << std::endl;
            char ch;
            while (file.available()) {
                ch = file.read();
                if (ch == '\n') {
                    //this->logger->_serialLog->println("Value: " + value);
                    this->SetSize(lineCount, value.toInt());
                    lineCount++;
                    value = "";
                    if (lineCount > 4) {
                        break;
                    }
                }else {
                    value += ch;
                }
            }
            file.close();
        }else {
            //std::cout << "File Open Failed" << std::endl;
        }
    }

    void ConfigurationReader::SetSize(int lineCount, int value) {
            switch (lineCount) {
            case 0: {
                this->DigitalInSize = value;
                break;
            }
            case 1: {
                this->AnalogInSize = value;
                break;
            }
            case 2: {
                this->ModuleSize = value;
                break;
            }
            case 3: {
                this->OutputSize = value;
                break;
            }
        }
    }

    bool ConfigurationReader::Init() {
        this->GetFileSizes();
        this->PrintSizes();
        return true;
    }

    std::vector<AnalogInputChannel> ConfigurationReader::DeserializeAnalogConfig() {
        if (this->AnalogInSize > 0) {
            DynamicJsonDocument doc(this->AnalogInSize);
            vector<AnalogInputChannel> analogChannels;
            File file = SD.open(AnalogFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if (error) {
                    return analogChannels;
                }else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        int chan = elem[F("Input")]; 
                        int reg = elem[F("Register")]; 
                        int slot = elem[F("Module Slot")];
                        bool connected = elem[F("Connected")];
                        AnalogInConfiguration config(chan, slot, reg, connected);
                        config.zeroValue = elem[F("ZeroValue")]; 
                        config.maxValue = elem[F("MaxValue")]; 
                        config.analogFactor = elem[F("AnalogFactor")]; 
                        config.bypassAlerts = elem[F("BypassAlerts")];
                        config.alerts.reserve(3);
                        JsonObject A1 = elem[F("A1")];
                        config.alerts[0].setpoint = A1[F("Setpoint")];
                        config.alerts[0].action = (AlertAction)A1[F("Action")];
                        config.alerts[0].bypass = A1[F("Bypass")];
                        config.alerts[0].enabled = A1[F("Enabled")];
                        config.alerts[0].prioirty = A1[F("Priority")];

                        JsonObject A2 = elem[F("A2")];
                        config.alerts[1].setpoint = A2[F("Setpoint")];
                        config.alerts[1].action = (AlertAction)A2[F("Action")];
                        config.alerts[1].bypass = A2[F("Bypass")];
                        config.alerts[1].enabled = A2[F("Enabled")];
                        config.alerts[1].prioirty = A2[F("Priority")];

                        JsonObject A3 = elem[F("A3")];
                        config.alerts[2].setpoint = A3[F("Setpoint")];
                        config.alerts[2].action = (AlertAction)A3[F("Action")];
                        config.alerts[2].bypass = A3[F("Bypass")];
                        config.alerts[2].enabled = A3[F("Enabled")];
                        config.alerts[2].prioirty = A3[F("Priority")];
                        std::sort(config.alerts.begin(), config.alerts.end(), [](const Alert& a, const Alert& b)->bool {
                            return a.prioirty > b.prioirty;
                        });
                        analogChannels.push_back(AnalogInputChannel(config));
                    }
                    file.close();
                    return analogChannels;
                }
            }else {
                return analogChannels;
            }
        }
    }

    std::vector<DiscreteOutputChannel> ConfigurationReader::DeserializeOutputConfig() {
        if (this->OutputSize > 0) {
            DynamicJsonDocument doc(this->OutputSize);
            std::vector<DiscreteOutputChannel> outputChannels;
            File file = SD.open(OutputFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if (error) {

                    //log error
                    return outputChannels;
                }
                else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        int chan = elem[F("Output")]; 
                        int reg = elem[F("Register")]; 
                        int slot = elem[F("Module Slot")];
                        bool connected = elem[F("Connected")];
                        OutputConfiguration config(chan,slot, reg, connected);
                        const char* Map_To_Channel = elem["Map To Channel"];

                        config.startState=(LogicType)elem["Start State"];
                        config.outputType=(OutputType)elem["Type"]; 
                        config.action = (OutputAction)elem[F("Action")]; 

                        string str= std::string(Map_To_Channel);
                        string delimiter = ",";
                        size_t pos = 0;
                        string token;
                        while ((pos = str.find(delimiter)) != std::string::npos) {
                            token = str.substr(0, pos);
                            int val = String(token.c_str()).toInt();
                            config.channelMaps.push_back(val);
                            str.erase(0, pos + delimiter.length());
                        }
                        outputChannels.push_back(config);
                    }
                    file.close();
                    return outputChannels;
                }
            }else {
                return outputChannels;
            }
        }
    }

    std::vector<DigitalInConfiguration> ConfigurationReader::DeserializeDigitalConfig() {
        if (this->DigitalInSize > 0) {
            DynamicJsonDocument doc(this->DigitalInSize);
            std::vector<DigitalInConfiguration>  configurations;
            File file = SD.open(DigitalFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if (error) {
                    return configurations;
                }else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        int chan = elem[F("Input")];
                        int slot = elem[F("Module Slot")];
                        int reg = elem[F("Coil")];
                        bool connected = elem[F("Connected")];
                        DigitalInConfiguration config(chan,slot, reg, connected);
                        config.Logic = (LogicType)elem[F("Logic")];
                        Alert alert;
                        JsonObject Alert = elem[F("Alert")];
                        alert.setpoint = Alert[F("Setpoint")];
                        alert.action = (AlertAction)Alert[F("Action")];
                        alert.bypass = Alert[F("Bypass")];
                        alert.enabled = Alert[F("Enabled")];
                        alert.prioirty = AlertPriority::Highest;
                        config.alert = alert;
                        configurations.push_back(config);
                    }
                    file.close();
                    return configurations;
                }
            }else {
                return configurations;
            }
        }
    }

    std::vector<ModuleConfiguration> ConfigurationReader::DeserializeModuleConfig() {
        DynamicJsonDocument doc(this->ModuleSize);
        std::vector<ModuleConfiguration> modules;
        File file = SD.open(ModuleFile);
        if (file) {
            DeserializationError error = deserializeJson(doc, file);
            if (error) {
                return modules;
            }
            else {
                int size = doc.as<JsonArray>().size();
                for (JsonObject elem : doc.as<JsonArray>()) {
                    ModuleConfiguration config;
                    config.moduleName = (Module)elem[F("Module")];
                    config.slot = elem[F("Slot")];
                    config.channelCount = elem[F("ChannelCount")];
                    config.channelMapEnd = elem[F("ChannelMap Stop")];
                    config.moduleType = (ModuleType)elem[F("Type")];
                    modules.push_back(config);
                }
                file.close();
                return modules;
                
            }
        }else {
            return modules;
        }
    }

    ConfigurationReader::~ConfigurationReader() {   }
};