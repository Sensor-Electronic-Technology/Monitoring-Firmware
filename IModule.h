#pragma once
#include "MonitoringComponent.h"
#include "Configuration.h"

namespace MonitoringComponents {
	struct Initialized {
		Initialized(bool configSet = false) :channelsSet(false), configSet(configSet) {}
		bool channelsSet;
		bool configSet;
		operator bool() const {
			return this->channelsSet && this->configSet;
		}
	};

	class IModule :public MonitoringComponent {
	public:
		IModule(ModuleConfiguration configuration, Ref<MonitoringComponent> parent = nullptr)
			:MonitoringComponent(parent),configuration(configuration), slot(configuration.slot), isInitialized(true) {	}
		IModule(Ref<MonitoringComponent> parent = nullptr) :MonitoringComponent(parent), isInitialized(false) { }
		virtual int Slot() = 0;
	protected:
		ModuleConfiguration configuration;
		int slot;
		Initialized isInitialized;
	};
};