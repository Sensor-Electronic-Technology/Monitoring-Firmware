#pragma once
#include <ArduinoSTL.h>
#include "P1AM.h"
#include "MonitoringComponent.h"
#include "DiscreteInputChannel.h"
#include "DiscreteOuputChannel.h"

namespace MonitoringComponents {
	struct Initialized {
		Initialized(bool configSet = false):channelsSet(false), configSet(configSet) {}
		bool channelsSet;
		bool configSet ;
		operator bool() const{
			return this->channelsSet && this->configSet;
		} 
	};

	class IModule :public MonitoringComponent {
	public:
		IModule(ModuleConfiguration configuration, Ref<MonitoringComponent> parent = nullptr)
			:configuration(configuration), slot(configuration.slot), isInitialized(true) {	}
		IModule(Ref<MonitoringComponent> parent=nullptr):MonitoringComponent(parent),isInitialized(){ }
		virtual int Slot() = 0;
	protected:
		ModuleConfiguration configuration;
		int slot;
		Initialized isInitialized;
	};

	class DiscreteInputModule :public IModule {
	public:
		DiscreteInputModule(ModuleConfiguration configuration, Ref<MonitoringComponent> parent = nullptr)
			:IModule(configuration,parent) {	}

		DiscreteInputModule(Ref<MonitoringComponent> parent = nullptr) 
			:IModule(parent){}

		int Slot();

		void CreateChannels(std::vector<DigitalInConfiguration> channelConfigurations);

		void SetConfig(ModuleConfiguration config);

		void Print();

		/*void OnChannelTrigger(DiscreteInputCallBack cbk);*/

		
	private:
		std::vector<DiscreteInputChannel*> channels;
		//ModuleConfiguration configuration;
		//int slot;
		Initialized isInitialized;

		void privateLoop();
	};

};

