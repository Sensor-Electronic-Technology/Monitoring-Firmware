#include "ConfigurationReader.h"

#define AnalogFile              "analog.txt"
#define OutputFile              "output.txt"
#define DigitalFile             "digital.txt"
#define ModuleFile              "module.txt"
#define ActionFile              "actions.txt"
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
        std::cout << AnalogFile << " " << this->AnalogInSize << std::endl;
        std::cout << OutputFile << " " << this->OutputSize << std::endl;
        std::cout << DigitalFile << " " << this->DigitalInSize << std::endl;
        std::cout << ActionFile << " " << this->ActionSize << std::endl;
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
                    std::cout << "DeserializeAnalog Failed" << endl;
                    return analogChannels;
                }else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        AnalogAlert alert1, alert2, alert3;

                        int input = elem[F("Input")]; 
                        ChannelAddress address;
                        address.channel = elem[F("Address")][F("Channel")];
                        address.slot = elem[F("Address")][F("Slot")];

                        int reg = elem[F("Register")];
                        int zeroValue = elem[F("ZeroValue")];
                        int maxValue = elem[F("MaxValue")];
                        int analogFactor = elem[F("AnalogFactor")];
                        int bypassAlerts = elem[F("BypassAlerts")];
                        int connected = elem[F("Connected")];

                        AnalogInConfiguration config(input,address, reg, connected);

                        JsonObject A1 = elem[F("A1")];
                        alert1.setPoint = A1[F("Setpoint")];
                        alert1.actionId = A1[F("Action")];
                        alert1.bypass = A1[F("Bypass")];
                        alert1.enabled = A1[F("Enabled")];
                        alert1.setPointFactor = 1;

                        JsonObject A2 = elem[F("A2")];
                        alert2.setPoint = A2[F("Setpoint")];
                        alert2.actionId = A2[F("Action")];
                        alert2.bypass = A2[F("Bypass")];
                        alert2.enabled = A2[F("Enabled")];
                        alert2.setPointFactor = 1;

                        JsonObject A3 = elem["A3"];
                        alert3.setPoint = A3[F("Setpoint")];
                        alert3.actionId = A3[F("Action")];
                        alert3.bypass = A3[F("Bypass")];
                        alert3.enabled = A3[F("Enabled")];
                        alert3.setPointFactor = 1;

                        config.alert1 = alert1;
                        config.alert2 = alert2;
                        config.alert3 = alert3;

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
                    std::cout << "Deserialize Output Failed" << endl;
                    return outputChannels;
                }
                else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        int channel = elem[F("Output")]; 
                        ChannelAddress address;
                        address.channel = elem[F("Addr")][F("Channel")]; 
                        address.slot = elem[F("Addr")][F("Module Slot")]; 
                        int reg = elem[F("Register")]; 
                        int connected = elem[F("Connected")];
                        State startState = (elem[F("Start State")].as<bool>() == true) ? State::High : State::Low;
                        OutputConfiguration config(channel,address,reg,startState,connected);
                        outputChannels.push_back(config);
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
                    std::cout << "Deserialize Digital Failed" << endl;
                    return configurations;
                }else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        
                        ChannelAddress address;

                        int input = elem[F("Input")];
                        address.channel = elem[F("Address")][F("Channel")];
                        address.slot = elem[F("Address")][F("Slot")];
                        int reg = elem[F("Coil")];
                        int connected = elem[F("Connected")];

                        DigitalInConfiguration config(input,address,reg,connected);

                        DigitalAlert alert;
                        JsonObject Alert = elem[F("Alert")];
                        alert.triggerOn = (Alert[F("TriggerOn")].as<bool>() == true) ? TriggerOn::High : TriggerOn::Low;
                        alert.actionId = Alert[F("Action")];
                        alert.actionType =(ActionType)Alert[F("ActionType")].as<int>();
                        alert.bypass = Alert[F("Bypass")].as<bool>();
                        alert.enabled = Alert[F("Enabled")].as<bool>();
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

    std::vector<ActionConfiguration> ConfigurationReader::DeserializeActions(){
        DynamicJsonDocument doc(this->ActionSize);
        File file = SD.open(ActionFile);
        std::vector<ActionConfiguration> actions;
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            std::cout << "Deserialize Action Failed" << std::endl;
            return actions;
        }
        for (JsonObject elem : doc.as<JsonArray>()) {
            ActionConfiguration config;
            config.actionId = elem["ActionId"]; // 0, 1, 2, 3, 4, 5
            config.actionType =(ActionType)elem["ActionType"].as<int>();
            JsonObject O1 = elem[F("O1")];

            config.addr1.channel = O1[F("Address")][F("Channel")];
            config.addr1.slot = O1[F("Address")][F("Slot")];
            config.onLevel1= (O1[F("OnLevel")].as<bool>()==true)? State::High:State::Low;
            config.offLevel1 = (O1[F("OffLevel")].as<bool>() == true) ? State::High : State::Low;

            JsonObject O2 = elem[F("O2")];

            config.addr2.channel = O2[F("Address")][F("Channel")];
            config.addr2.slot = O2[F("Address")][F("Slot")];
            config.onLevel2 = (O2[F("OnLevel")].as<bool>() == true) ? State::High : State::Low;
            config.offLevel2 = (O2[F("OffLevel")].as<bool>() == true) ? State::High : State::Low;

            JsonObject O3 = elem[F("O3")];

            config.addr3.channel = O3[F("Address")][F("Channel")];
            config.addr3.slot = O3[F("Address")][F("Slot")];
            config.onLevel3 = (O3[F("OnLevel")].as<bool>() == true) ? State::High : State::Low;
            config.offLevel3 = (O3[F("OffLevel")].as<bool>() == true) ? State::High : State::Low;

            //config.addr2.channel = elem[F("O2")][F("Address")][F("Channel")];
            //config.addr2.slot = elem[F("O2")][F("Address")][F("Slot")];
            //config.onLevel2 = (elem[F("O2")][F("Level")].as<bool>() == true) ? State::High : State::Low;

            //config.addr3.channel = elem[F("O3")][F("Address")][F("Channel")];
            //config.addr3.slot = elem[F("O3")][F("Address")][F("Slot")];
            //config.onLevel3 = (elem[F("O3")][F("Level")].as<bool>() == true) ? State::High : State::Low;

            config.startState= (elem[F("Start State")].as<bool>()==true)? State::High:State::Low;
            config.type = (OutputType)elem[F("OutputType")];
            config.modbusAddress.address = elem[F("ModbusRegister")];
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
        file.close();
        return actions;
    }

    ConfigurationReader::~ConfigurationReader() {   }
};