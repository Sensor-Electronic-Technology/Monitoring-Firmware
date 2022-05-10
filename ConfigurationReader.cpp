#include "ConfigurationReader.h"


#define AnalogFile              "analog.txt"
#define OutputFile              "output.txt"
#define DigitalFile             "digital.txt"
#define VirtualFile             "virtual.txt"
#define ModuleFile              "module.txt"
#define ActionFile              "actions.txt"
#define NetConfigFile           "network.txt"
#define SizeFile                "size.txt"
#define LOG_FILENAME            "log.txt"

namespace MonitoringComponents {
    ConfigurationReader::ConfigurationReader() {
        this->configLoaded = false;
        this->DigitalInSize = 0;
        this->OutputSize = 0;
        this->AnalogInSize = 0;
        this->ModuleSize = 0;
        this->NetConfigSize = 0;
    }

    void ConfigurationReader::PrintSizes() {
        std::cout << AnalogFile << " " << this->AnalogInSize << std::endl;
        std::cout << OutputFile << " " << this->OutputSize << std::endl;
        std::cout << DigitalFile << " " << this->DigitalInSize << std::endl;
        std::cout << VirtualFile << " " << this->VirtualSize << std::endl;
        std::cout << ActionFile << " " << this->ActionSize << std::endl;
        std::cout << NetConfigFile << " " << this->NetConfigSize << std::endl;
        std::cout << ModuleFile << " " << this->ModuleSize << std::endl;
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
                    this->SetSize(lineCount, value.toInt());
                    lineCount++;
                    value = "";
                    if (lineCount > 6) {
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

    void ConfigurationReader::CreatConfigFiles(){
        if (!SD.begin(SDCARD_SS_PIN)) {
            while (1);
        }
        std::cout << "SD Card Initialized" << std::endl;
        File file;

        file = SD.open(AnalogFile,FILE_WRITE);
        if (file) {
            std::cout << AnalogFile << " Created!" << std::endl;
            file.close();
        }
        file = SD.open(DigitalFile,FILE_WRITE);
        if (file) {
            std::cout << DigitalFile << " Created!" << std::endl;
            file.close();
        }

        file = SD.open(VirtualFile, FILE_WRITE);
        if (file) {
            std::cout << VirtualFile << "Created!" << std::endl;
            file.close();
        }

        file = SD.open(ActionFile,FILE_WRITE);
        if (file) {
            std::cout << ActionFile << " Created!" << std::endl;
            file.close();
        }

        file = SD.open(OutputFile,FILE_WRITE);
        if (file) {
            std::cout << OutputFile << " Created!" << std::endl;
            file.close();
        }

        file = SD.open(NetConfigFile,FILE_WRITE);
        if (file) {
            std::cout << NetConfigFile << " Created!" << std::endl;
            file.close();
        }

        file = SD.open(ModuleFile, FILE_WRITE);
        if (file) {
            std::cout << ModuleFile << "Created!" << std::endl;
        }

        file = SD.open(SizeFile, FILE_WRITE);
        if (file) {
            std::cout << SizeFile << " Created!" << std::endl;
            file.close();
        }

        file = SD.open(LOG_FILENAME, FILE_WRITE);
        if (file) {
            std::cout << LOG_FILENAME << " Created!" << std::endl;
            file.close();
        }

        std::cout << "Files should be created..." << std::endl;
    }

    char ConfigurationReader::nibbleTobyte(char c){
        if ((c >= '0') && (c <= '9'))
            return c - '0';
        if ((c >= 'A') && (c <= 'F'))
            return c + 10 - 'A';
        if ((c >= 'a') && (c <= 'a'))
            return c + 10 - 'a';
        return -1;
    }

    char ConfigurationReader::ToHex(char c1, char c2){
        if (ConfigurationReader::nibbleTobyte(c2) >= 0)
            return ConfigurationReader::nibbleTobyte(c1) * 16 + ConfigurationReader::nibbleTobyte(c2);
        return ConfigurationReader::nibbleTobyte(c1);
    }

    std::string ConfigurationReader::HexToString(char* data) {
        std::string result = "";
        for (int i = 0; ConfigurationReader::nibbleTobyte(data[i]) >= 0; i++){
            result += ConfigurationReader::ToHex(data[i], data[i + 1]);
            if (ConfigurationReader::nibbleTobyte(data[i + 1]) >= 0)
                i++;
        }
        return result;
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
                this->OutputSize = value;
                break;
            }
            case 3: {
                this->ActionSize = value;
                break;
            }
            case 4: {
                this->NetConfigSize = value;
                break;
            }

            case 5: {
                this->VirtualSize = value;
                break;
            }

            case 6:{
                this->ModuleSize = value;
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
                    std::cout << F("DeserializeAnalog Failed") << endl;
                    file.close();
                    doc.garbageCollect();
                    return analogChannels;
                }else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        AnalogAlert alert1, alert2, alert3;

                        int input = elem[F("Input")]; 
                        ChannelAddress address;
                        address.channel = elem[F("Address")][F("Channel")];
                        address.slot = elem[F("Address")][F("Slot")];

                        ModbusAddress modbusAddress;
                        modbusAddress.address=elem[F("MRI")][F("Register")].as<int>();
                        modbusAddress.type=(RegisterType)elem[F("MRI")][F("Type")].as<int>();

                        ModbusAddress alertAddress;
                        alertAddress.address=elem[F("MRA")][F("Register")].as<int>();
                        alertAddress.type=(RegisterType)elem[F("MRA")][F("Type")].as<int>();

                        int reg = elem[F("Register")];
                        float slope = elem[F("Slope")].as<float>();
                        float offset = elem[F("Offset")].as<float>();

                        int analogFactor = elem[F("AnalogFactor")];
                        int bypassAlerts = elem[F("BypassAlerts")];
                        int connected = elem[F("Connected")];
                        
                        AnalogInConfiguration config(input,address, modbusAddress,alertAddress, connected);
                        config.slope = slope;
                        config.offset = offset;
                        config.bypassAlerts = bypassAlerts;
                        config.analogFactor = analogFactor;

                        JsonObject A1 = elem[F("A1")];
                        alert1.setPoint = A1[F("Setpoint")];
                        alert1.actionId = A1[F("Action")];
                        alert1.actionType=(ActionType)A1[F("ActionType")].as<int>();
                        alert1.bypass = A1[F("Bypass")];
                        alert1.enabled = A1[F("Enabled")];
                        alert1.setPointFactor = 1;

                        JsonObject A2 = elem[F("A2")];
                        alert2.setPoint = A2[F("Setpoint")];
                        alert2.actionId = A2[F("Action")];
                        alert2.actionType=(ActionType)A2[F("ActionType")].as<int>();
                        alert2.bypass = A2[F("Bypass")];
                        alert2.enabled = A2[F("Enabled")];
                        alert2.setPointFactor = 1;

                        JsonObject A3 = elem["A3"];
                        alert3.setPoint = A3[F("Setpoint")];
                        alert3.actionId = A3[F("Action")];
                        alert3.actionType=(ActionType)A3[F("ActionType")].as<int>();
                        alert3.bypass = A3[F("Bypass")];
                        alert3.enabled = A3[F("Enabled")];
                        alert3.setPointFactor = 1;

                        config.alert1 = alert1;
                        config.alert2 = alert2;
                        config.alert3 = alert3;

                        analogChannels.push_back(config);
                    }
                    file.close();
                    doc.garbageCollect();
                    return analogChannels;
                }
            }else {
                cout << F("Failed to open AnalogConfig File") << endl;
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
                    std::cout << F("Deserialize Output Failed") << endl;
                    file.close();
                    doc.garbageCollect();
                    return outputChannels;
                }
                else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        int channel = elem[F("Output")]; 
                        ChannelAddress address;
                        address.channel = elem[F("Addr")][F("Channel")]; 
                        address.slot = elem[F("Addr")][F("Module Slot")]; 
                        ModbusAddress modbusAddress;
                        modbusAddress.address=elem[F("MRI")][F("Register")].as<int>(); 
                        modbusAddress.type=(RegisterType)elem[F("MRI")][F("Type")].as<int>(); 
                        int connected = elem[F("Connected")];
                        State startState = (elem[F("Start State")].as<bool>() == true) ? State::High : State::Low;
                        OutputConfiguration config(channel,address,modbusAddress,startState,connected);
                        outputChannels.push_back(config);
                    }
                    file.close();
                    doc.garbageCollect();
                    return outputChannels;
                }
            }else {
                std::cout << F("Failed to opend OutputConfig File") << std::endl;
                return outputChannels;
            }
        }
    }

    std::vector<DigitalInConfiguration> ConfigurationReader::DeserializeDigitalConfig() {
        std::vector<DigitalInConfiguration>  configurations;
        if (this->DigitalInSize > 0) {
            DynamicJsonDocument doc(this->DigitalInSize);
            File file = SD.open(DigitalFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if (error) {
                    std::cout << F("Deserialize Digital Failed") << endl;
                    file.close();
                    doc.garbageCollect();
                    return configurations;
                }else {
                    size_t size = doc.as<JsonArray>().size();
                    for (JsonObject elem : doc.as<JsonArray>()) {
                        ChannelAddress address;
                        int input = elem[F("Input")];
                        address.channel = elem[F("Address")][F("Channel")];
                        address.slot = elem[F("Address")][F("Slot")];
                        int connected = elem[F("Connected")];
                        ModbusAddress modbusAddress;
                        modbusAddress.address=elem[F("MRI")][F("Register")].as<int>();
                        modbusAddress.type=(RegisterType)elem[F("MRI")][F("Type")].as<int>();

                        ModbusAddress alertAddress;
                        alertAddress.address=elem[F("MRA")][F("Register")].as<int>();
                        alertAddress.type=(RegisterType)elem[F("MRA")][F("Type")].as<int>();

                        DigitalInConfiguration config(input,address,modbusAddress,alertAddress,connected);
                        DigitalAlert alert;
                        JsonObject Alert = elem[F("Alert")];
                        alert.triggerOn = (Alert[F("TriggerOn")].as<bool>() == true) ? TriggerOn::High : TriggerOn::Low;
                        config.triggerOn=alert.triggerOn;
                        alert.actionId = Alert[F("Action")];
                        alert.actionType = (ActionType)Alert[F("ActionType")].as<int>();
                        alert.bypass = Alert[F("Bypass")].as<bool>();
                        alert.enabled = Alert[F("Enabled")].as<bool>();   
                        if (alert.actionId == -1 || ((int)alert.actionType < 1 || (int)alert.actionType>6)) {
                            alert.enabled = false;
                        }
                        config.alert = alert;
                        configurations.push_back(config);
                    }
                    file.close();
                    doc.garbageCollect();
                    return configurations;
                }
            }else {
                doc.garbageCollect();
                cout << F("Failed to opend DigitalConfig file") << endl;
                return configurations;
            }
        } else {
            return configurations;
        }
    }

    std::vector<VirtualDigitalConfiguration> ConfigurationReader::DeserializeVirtualConfig() {
        std::vector<VirtualDigitalConfiguration> configurations;
        if (this->VirtualSize > 0) {
            DynamicJsonDocument doc(this->VirtualSize);
            File file = SD.open(VirtualFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if (error) {
                    std::cout << F("Deserialize Digital Failed") << endl;
                    file.close();
                    doc.garbageCollect();
                    return configurations;
                } else {
                    size_t size = doc.as<JsonArray>().size();
                    for (auto elem : doc.as<JsonArray>()) {
                        int input = elem[F("Input")]; 
                        bool enabled = elem[F("Connected")]; 

                        ModbusAddress modbusAddress;
                        modbusAddress.address=elem[F("MRI")][F("Register")].as<int>();
                        modbusAddress.type=(RegisterType)elem[F("MRI")][F("Type")].as<int>();

                        ModbusAddress alertAddress;
                        alertAddress.address=elem[F("MRA")][F("Register")].as<int>();
                        alertAddress.type=(RegisterType)elem[F("MRA")][F("Type")].as<int>();

                        VirtualDigitalConfiguration config(input,modbusAddress,alertAddress,enabled); 

                        DigitalAlert alert;
                        JsonObject Alert = elem[F("Alert")];
                        alert.triggerOn = (Alert[F("TriggerOn")].as<bool>() == true) ? TriggerOn::High : TriggerOn::Low;
                        alert.actionId = Alert[F("Action")];
                        alert.actionType = (ActionType)Alert[F("ActionType")].as<int>();
                        alert.bypass = Alert[F("Bypass")].as<bool>();
                        alert.enabled = Alert[F("Enabled")].as<bool>();
                        if (alert.actionId == -1 || ((int)alert.actionType < 1 || (int)alert.actionType>6)) {
                            alert.enabled = false;
                        }
                        config.alert = alert;
                        configurations.push_back(config);
                    }
                    file.close();
                    doc.garbageCollect();
                    return configurations;
                }
            } else {
                doc.garbageCollect();
                return configurations;
            }
        } else {
            std::cout << "Error: VirtualDiscreteInput File size is 0, failed to reas configuration" << endl;
            return configurations;
        }
    }

    std::vector<const char*> ConfigurationReader::DeserializeModuleConfig() {
        DynamicJsonDocument doc(this->ModuleSize);
        std::vector<const char*> modules;
        File file = SD.open(ModuleFile);
        if (file) {
            DeserializationError error = deserializeJson(doc, file);
            if (error) {
                file.close();
                doc.garbageCollect();
                return modules;
            }
            else {
                int size = doc.as<JsonArray>().size();
                for (auto elem : doc.as<JsonArray>()) {
                    const char* mod = doc[F("Module")];
                    modules.push_back(mod);
                }
                file.close();
                doc.garbageCollect();
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
            std::cout << F("Deserialize Action Failed") << std::endl;
            file.close();
            doc.garbageCollect();
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

            config.startState= (elem[F("Start State")].as<bool>()==true)? State::High:State::Low;
            config.type = (OutputType)elem[F("OutputType")];
            config.modbusAddress.address = elem[F("Register")];
            config.modbusAddress.type = RegisterType::DiscreteInput;
            actions.push_back(config);
        }
        file.close();
        doc.garbageCollect();
        return actions;
    }

    NetConfiguration ConfigurationReader::DeserializeNetConfiguration() {
        NetConfiguration netConfig;
        if (this->NetConfigSize > 0) {
            DynamicJsonDocument doc(this->NetConfigSize);
            File file = SD.open(NetConfigFile);
            if (file) {
                DeserializationError error = deserializeJson(doc, file);
                if(error) {
                    cout << F("Error deserializing netconfig") << endl;
                    file.close();
                    doc.garbageCollect();
                    return netConfig;
                }
                JsonObject root_0 = doc[0];
                const char* IP=root_0[F("IP")];
                const char* DNS = root_0[F("DNS")];
                const char* Mac = root_0[F("Mac")]; 
                const char* Gateway = root_0[F("Gateway")];
                netConfig.inputRegisters = root_0[F("InputRegsters")].as<int>();
                netConfig.coils= root_0[F("Coils")].as<int>(); 
                netConfig.discreteInputs = root_0[F("DiscreteInputs")].as<int>();
                netConfig.holdingRegisters=root_0[F("HoldingRegister")].as<int>();
                ModbusAddress modbusAddress;
                modbusAddress.address=root_0[F("Register")];
                modbusAddress.type=(RegisterType)root_0[F("Type")].as<int>();
                netConfig.modbusAddress=modbusAddress;
                netConfig.ip.fromString(IP);
                netConfig.gateway.fromString(DNS);
                netConfig.gateway.fromString(Gateway);
                netConfig.mac[0]=  ConfigurationReader::ToHex(Mac[0], Mac[1]);
                netConfig.mac[1] = ConfigurationReader::ToHex(Mac[2], Mac[3]);
                netConfig.mac[2] = ConfigurationReader::ToHex(Mac[4], Mac[5]);
                netConfig.mac[3] = ConfigurationReader::ToHex(Mac[6], Mac[7]);
                netConfig.mac[4] = ConfigurationReader::ToHex(Mac[8], Mac[9]);
                netConfig.mac[5] = ConfigurationReader::ToHex(Mac[10], Mac[11]);
                
                file.close();
                doc.garbageCollect();
                return netConfig;
            } else {
                std::cout << F("Error opening netconfig") << std::endl;
                doc.garbageCollect();
                return netConfig;
            }
        } else {
            std::cout << F("Error: NetConfig file size=0, failed to open") << std::endl;
            return netConfig;
        }
    }

    ConfigurationReader::~ConfigurationReader() {   }
};