#pragma once
#include <ArduinoSTL.h>
#include <P1AM.h>
#include "MonitoringComponent.h"
#include "Callbacks.h"
#include "Ref.h"

namespace MonitoringComponents {
	class Timer : public MonitoringComponent {
	public:
		Timer(Ref<MonitoringComponent> parent);
		Timer() : Timer(nullptr) {}
		void setTimeout(VoidCallback callback, unsigned long delay);
		void onInterval(VoidCallback callback, unsigned long interval);
		bool isRunning();
		unsigned long timeLeft();
		void cancel();

	private:
		void privateLoop();
		void Print(){}

		VoidCallback _timeout_callback;
		bool _timer_running;
		unsigned long _deadline;

		unsigned _interval;
		bool _intervaling;
	};
};


