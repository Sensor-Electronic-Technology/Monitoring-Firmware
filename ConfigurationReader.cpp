#include "ConfigurationReader.h"

#define AnalogFile              "analog.txt"
#define OutputFile              "output.txt"
#define DigitalFile             "digital.txt"
#define ModuleFile              "module.txt"
#define ActionFile              "action.txt"
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
        File file = SD.open(SizeFile);
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
            //case 2: {
            //    this->ModuleSize = value;
            //    break;
            //}
            case 2: {
                this->OutputSize = value;
                break;
            }
            case 3: {
                this->ActionSize = value;
                break;
            }
        }
    }

    bool ConfigurationReader::Init() {
        this->GetFileSizes();
        this->PrintSizes();
        return true;
    }

    std::vector<AnalogInConfiguration> ConfigurationReader::DeserializeAnalogConfig() {
        if (this->AnalogInSize > 0) {
            DynamicJsonDocument doc(this->AnalogInSize);
            vector<AnalogInConfiguration> analogChannels;
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

                        //int Input = elem["Input"]; // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16

                        //int Address_Channel = elem["Address"]["Channel"]; // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, ...
                        //int Address_Slot = elem["Address"]["Slot"]; // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1

                        //int Register = elem["Register"]; // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
                        //int ZeroValue = elem["ZeroValue"]; // 4, 4, -20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int MaxValue = elem["MaxValue"]; // 20, 20, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int AnalogFactor = elem["AnalogFactor"]; // 10000, 10000, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int BypassAlerts = elem["BypassAlerts"]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int Connected = elem["Connected"]; // 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

                        //JsonObject A1 = elem["A1"];
                        //float A1_Setpoint = A1["Setpoint"]; // 15.56, 10.65, -20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A1_Action = A1["Action"]; // 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A1_Bypass = A1["Bypass"]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A1_Enabled = A1["Enabled"]; // 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

                        //JsonObject A2 = elem["A2"];
                        //float A2_Setpoint = A2["Setpoint"]; // 17.5, 12.6, -18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A2_Action = A2["Action"]; // 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A2_Bypass = A2["Bypass"]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A2_Enabled = A2["Enabled"]; // 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

                        //JsonObject A3 = elem["A3"];
                        //float A3_Setpoint = A3["Setpoint"]; // 19.65, 15.95, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A3_Action = A3["Action"]; // 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A3_Bypass = A3["Bypass"]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        //int A3_Enabled = A3["Enabled"]; // 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        analogChannels.push_back(config);
                    }
                    file.close();
                    return analogChannels;
                }
            }else {
                return analogChannels;
            }
        }
    }

    std::vector<OutputConfiguration> ConfigurationReader::DeserializeOutputConfig() {
        if (this->OutputSize > 0) {
            DynamicJsonDocument doc(this->OutputSize);
            std::vector<OutputConfiguration> outputChannels;
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

                        //config.startState=(LogicType)elem["Start State"];
                        //config.outputType=(OutputType)elem["Type"]; 
                        //config.action = (OutputAction)elem[F("Action")]; 

                        //string str= std::string(Map_To_Channel);
                        //string delimiter = ",";
                        //size_t pos = 0;
                        //string token;
                        //while ((pos = str.find(delimiter)) != std::string::npos) {
                        //    token = str.substr(0, pos);
                        //    int val = String(token.c_str()).toInt();
                        //    config.channelMaps.push_back(val);
                        //    str.erase(0, pos + delimiter.length());
                        //}
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

                        //DigitalInConfiguration config(chan,slot, reg, connected);
                        int Input = elem["Input"]; // 1, 2, 3, 4, 5, 6, 7, 8

                        int Address_Channel = elem["Address"]["Channel"]; // 1, 2, 3, 4, 5, 6, 7, 8
                        int Address_Slot = elem["Address"]["Slot"]; // 1, 1, 1, 1, 1, 1, 1, 1

                        int Coil = elem["Coil"]; // 0, 1, 2, 3, 4, 5, 6, 7
                        int Connected = elem["Connected"]; // 1, 1, 1, 1, 1, 1, 1, 1

                        JsonObject Alert = elem["Alert"];
                        int Alert_TriggerOn = Alert["TriggerOn"]; // 1, 0, 0, 0, 1, 0, 1, 0
                        int Alert_Action = Alert["Action"]; // 1, 1, 4, 5, 3, 1, 1, 1
                        int Alert_Bypass = Alert["Bypass"]; // 0, 0, 0, 0, 0, 0, 0, 0
                        int Alert_Enabled = Alert["Enabled"]; // 1, 1, 1, 1, 1, 1, 1, 1

                        //int Output = elem["Output"]; // 1, 2, 3, 4, 5, 6, 7, 8

                        //int Addr_Channel = elem["Addr"]["Channel"]; // 1, 2, 3, 4, 5, 6, 7, 8
                        //int Addr_Module_Slot = elem["Addr"]["Module Slot"]; // 2, 2, 2, 2, 2, 2, 2, 2

                        //int Register = elem["Register"]; // 0, 1, 2, 3, 4, 5, 6, 7
                        //int Start_State = elem["Start State"]; // 0, 0, 0, 0, 0, 0, 0, 0
                        //int Connected = elem["Connected"]; // 1, 1, 1, 1, 0, 0, 0, 0

                       // configurations.push_back(config);
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

    std::vector<ActionConfiguration> ConfigurationReader::DeserializeActions(){
        DynamicJsonDocument doc(this->ActionSize);
        File file = SD.open(ActionFile);
        std::vector<ActionConfiguration> actions;
        DeserializationError error = deserializeJson(doc, file);

        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return actions;
        }
        for (JsonObject elem : doc.as<JsonArray>()) {
            ActionConfiguration config;
            config.actionId = elem["ActionId"]; // 0, 1, 2, 3, 4, 5

            config.addr1.channel = elem["O1"]["Address"]["Channel"]; 
            config.addr1.slot = elem["O1"]["Address"]["Slot"];
            config.outputlevel1= (elem["O1"]["Level"].as<bool>()==true)? State::High:State::Low; 

            config.addr2.channel = elem["O2"]["Address"]["Channel"];
            config.addr2.slot = elem["O2"]["Address"]["Slot"];
            config.outputlevel2 = (elem["O2"]["Level"].as<bool>() == true) ? State::High : State::Low;

            config.addr3.channel = elem["O3"]["Address"]["Channel"];
            config.addr3.slot = elem["O3"]["Address"]["Slot"];
            config.outputlevel3 = (elem["O3"]["Level"].as<bool>() == true) ? State::High : State::Low;

            config.startState= (elem["Start State"].as<bool>()==true)? State::High:State::Low; 
            config.type = (OutputType)elem["OutputType"]; 
            config.modbusAddress.address = elem["ModbusRegister"]; 
            config.modbusAddress.type = RegisterType::DiscreteInput;
            actions.push_back(config);

            //int ActionId = elem["ActionId"]; // 0, 1, 2, 3, 4, 5

            //int O1_Address_Channel = elem["O1"]["Address"]["Channel"]; // 1, 2, 3, 0, 3, 6
            //int O1_Address_Slot = elem["O1"]["Address"]["Slot"]; // 2, 2, 2, 0, 2, 2

            //int O1_Level = elem["O1"]["Level"]; // 1, 1, 1, 0, 1, 0

            //int O2_Address_Channel = elem["O2"]["Address"]["Channel"]; // 1, 2, 3, 0, 3, 6
            //int O2_Address_Slot = elem["O2"]["Address"]["Slot"]; // 1, 2, 3, 0, 3, 6

            //int O2_Level = elem["O2"]["Level"]; // 0, 0, 0, 0, 1, 0

            //int O3_Address_Channel = elem["O3"]["Address"]["Channel"]; // 1, 2, 3, 0, 3, 6
            //int O3_Address_Slot = elem["O3"]["Address"]["Slot"]; // 1, 2, 3, 0, 3, 6

            //int O3_Level = elem["O3"]["Level"]; // 0, 0, 0, 0, 0, 0

            //int Start_State = elem["Start State"]; // 0, 0, 0, 0, 0, 0
            //int OutputType = elem["OutputType"]; // 0, 0, 0, 0, 0, 2
            //int ModbusRegister = elem["ModbusRegister"]; // 1, 2, 3, 4, 5, 6
        }
        return actions;
    }

    ConfigurationReader::~ConfigurationReader() {   }
};