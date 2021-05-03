#pragma once
#include <ArduinoSTL.h>
#include "Data.h"
#include "ActionOutput.h"

namespace MonitoringComponents {
	class Action {
	public:
		Action() {	}

		Action(ActionConfiguration configuration) {
			this->actionId = configuration.actionId;
			this->actionType = configuration.actionType;
			this->initialState = configuration.startState;
			this->address = configuration.modbusAddress;
		}

		int Id() {
			return this->actionId;
		}

		void Invoke() {
			if (this->output1 != nullptr) {
				this->output1->TriggerOutput();
			}

			if (this->output2 != nullptr) {
				this->output2->TriggerOutput();
			}

			if (this->output3 != nullptr) {
				this->output3->TriggerOutput();
			}
		}

		void Clear() {
			if (this->output1 != nullptr) {
				this->output1->Reset();
			}

			if (this->output2 != nullptr) {
				this->output2->Reset();
			}

			if (this->output3 != nullptr) {
				this->output3->Reset();
			}
		}

		void SetOutput(ActionOutput* output, int outputNumber) {
			if (outputNumber == 1) {
				this->output1 = output;
			} else if (outputNumber == 2) {
				this->output2 = output;
			} else if (outputNumber == 3) {
				this->output3 = output;
			}
		}

	private:
		int actionId;
		ActionType actionType;
		OutputType type;
		State initialState;
		ModbusAddress address;


		ActionOutput* output1;
		ActionOutput* output2;
		ActionOutput* output3;
	};
};
