#pragma once
#include <ArduinoSTL.h>
#include "Module.h"
#include "Configuration.h"
#include "ConfigurationReader.h"

class MonitoringController{
public:
	MonitoringController() {

	}

	void Run() {

	}

	std::vector<DiscreteInputModule> mDiscreteIn;
	std::vector<DiscreteOutputModule> mDiscreteOut;
private:



};

