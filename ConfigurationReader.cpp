#include "ConfigurationReader.h"


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

    void ConfigurationReader::GetFileSizes() {
        File file = SD.open("size.txt");
        String value = "";
        int lineCount = 0;
        if (file) {
            char ch;
            while (file.available()) {
                ch = file.read();
                if (ch == '\n') {
                    this->logger->_serialLog->println("Value: " + value);
                    this->SetSize(lineCount, value.toInt());
                    lineCount++;
                    value = "";
                    if (lineCount > 4) {
                        break;
                    }
                }
                else {
                    value += ch;
                }
            }
            this->logger->_serialLog->println(F("Closing Size File"));
            file.close();
        }
        else {
            this->logger->error(F("Error: Could not open file!"));
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

    void ConfigurationReader::PrintSizes() {
        this->logger->_serialLog->println("Digital: " + String(this->DigitalInSize));
        this->logger->_serialLog->println("Analog: " + String(this->AnalogInSize));
        this->logger->_serialLog->println("Module: " + String(this->ModuleSize));
        this->logger->_serialLog->println("Output: " + String(this->OutputSize));
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
                        int chan = elem[F("Input")]; // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, ...
                        int reg = elem[F("Register")]; // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, ...
                        int slot = elem[F("Module Slot")]; // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, ...
                        bool connected = elem[F("Connected")];
                        AnalogInConfiguration config(chan, slot, reg, connected);
                        config.zeroValue = elem[F("ZeroValue")]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        config.maxValue = elem[F("MaxValue")]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        config.analogFactor = elem[F("AnalogFactor")]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        config.bypassAlerts = elem[F("BypassAlerts")]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
                    this->logger->_serialLog->println(F("OutputFile Deserializejson error: "));
                    this->logger->_serialLog->println(error.f_str());
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
                        config.connected = elem[F("Connected")]; 
                        outputChannels.push_back(config);
                        String str = String(Map_To_Channel);
                        char* temp=strtok((char*)Map_To_Channel, ",");
                        int i = 0;
                        while (temp != NULL) {
                            int val = String(temp).toInt();
                            config.channelMaps.push_back(val);
                            temp = strtok(NULL, ",");
                        }


                        //int Output = elem["Output"]; // 1, 2, 3, 4, 5, 6, 7, 8
                        //int Module_Slot = elem["Module Slot"]; // 2, 2, 2, 2, 2, 2, 2, 2

                        //int Register_Coil = elem["Register"; // 0, 1, 2, 3, 4, 5, 6, 7

                        //int Start_State = elem["Start State"]; // 0, 0, 1, 1, 1, 1, 1, 1
                        //int Type = elem["Type"]; // 0, 0, 0, 3, 3, 3, 3, 3
                        //int Action = elem["Action"]; // 4, 3, 2, 1, 0, 0, 0, 0
                        //const char* Map_To_Channel = elem["Map To Channel"]; // "0", "0", "0", "4,5,6,7,8", "0", "0", "0", "0"
                        //int Connected = elem["Connected"]; // 1, 1, 1, 1, 0, 0, 0, 0
                    }
                    file.close();
                    return outputChannels;
                }
            }else {
                return outputChannels;
            }
        }
    }

    std::vector<DiscreteInputChannel> ConfigurationReader::DeserializeDigitalConfig() {
        if (this->DigitalInSize > 0) {
            DynamicJsonDocument doc(this->DigitalInSize);
            std::vector<DiscreteInputChannel>  digitalChannels;
            File file = SD.open(DigitalFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if (error) {
                    return digitalChannels;
                }
                else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        int chan = elem[F("Input")]; // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, ...
                        int slot = elem[F("Module Slot")];
                        int reg = elem[F("Coil")]; // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, ...
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
                        //NEW
                        //int Input = elem["Input"]; // 1, 2, 3, 4, 5, 6, 7, 8
                        //int Module_Slot = elem["Module Slot"]; // 1, 1, 1, 1, 1, 1, 1, 1
                        //int Coil = elem["Coil"]; // 0, 1, 2, 3, 4, 5, 6, 7
                        //int Logic = elem["Logic"]; // 1, 0, 0, 1, 0, 1, 1, 0
                        //int Connected = elem["Connected"]; // 1, 1, 1, 1, 1, 1, 1, 1

                        //JsonObject Alert = elem["Alert"];
                        //int Alert_Setpoint = Alert["Setpoint"]; // 0, 0, 0, 0, 0, 0, 0, 0
                        //int Alert_Action = Alert["Action"]; // 4, 4, 0, 0, 3, 4, 4, 3
                        //int Alert_Bypass = Alert["Bypass"]; // 1, 1, 1, 1, 1, 1, 1, 1
                        //int Alert_Enabled = Alert["Enabled"]; // 0, 0, 0, 0, 0, 0, 0, 0

                        digitalChannels.push_back(DiscreteInputChannel(config));
                    }
                    file.close();
                    return digitalChannels;
                }
            }else {
                return digitalChannels;
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