#pragma once
#include "Ref.h"
#include "Function.h"
#include "Callbacks.h"

namespace MonitoringComponents {
	class DiscreteInputChannel;
	class AnalogInputChannel;
	typedef Function<void(void)> VoidCallback;
	typedef Function<void(Ref<DiscreteInputChannel>)> DiscreteInputCallBack;
	typedef Function<void(Ref<AnalogInputChannel>)> AnalogInputCallBack;

};