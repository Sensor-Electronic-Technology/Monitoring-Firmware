#pragma once
#include <ArduinoSTL.h>
#include "MonitoringComponent.h"
#include "DiscreteInputChannel.h"
namespace MonitoringComponents {
	class IModule:public MonitoringComponent {
	public:
		IModule(ModuleConfiguration configuration) :configuration(configuration), slot(configuration.slot) {	}
		virtual int Slot() = 0;
		virtual void Read() = 0;
		virtual void Initialize() = 0;
		virtual void Print() = 0;
	protected:
		ModuleConfiguration configuration;
		int slot;
	};

	class DiscreteInputModule :public IModule {
	public:
		DiscreteInputModule(ModuleConfiguration configuration) :IModule(configuration){	}

		virtual int Slot()override {
			return this->slot;
		}

		virtual void Read()override {
			std::cout << "DiscreteInputModule Read()" << endl;
		}

		virtual void Initialize() override{
			std::cout << "DiscreteInputModule Initialize()" << endl;
		}

		void Print() override {
			std::cout << "Module Name: " << this->configuration.moduleName;
			std::cout << " Module Type: " << this->configuration.moduleType;
			std::cout << " Slot: " << this->configuration.slot;
			std::cout << " ChannelCount: " << this->configuration.channelCount<<std::endl;
		}

		
	private:
		std::vector<DiscreteInputChannel> channels;
		virtual void privateLoop()override {

		}
	};

	class DiscreteOutputModule :public IModule {
	public:
		DiscreteOutputModule(ModuleConfiguration configuration) :IModule(configuration) {	}

		virtual int Slot()override {
			return this->slot;
		}

		virtual void Read()override {
			std::cout << "DiscreteInputModule Read()" << endl;
		}

		virtual void Initialize() override {
			std::cout << "DiscreteInputModule Initialize()" << endl;
		}

		void Print() override {
			std::cout << "Module Name: " << this->configuration.moduleName;
			std::cout << " Module Type: " << this->configuration.moduleType;
			std::cout << " Slot: " << this->configuration.slot;
			std::cout << " ChannelCount: " << this->configuration.channelCount << std::endl;
		}


	private:
		std::vector<DiscreteOutputChannel> channels;
		virtual void privateLoop()override {

		}
	};
};

