#pragma once
#include <ArduinoSTL.h>
#include "MonitoringComponent.h"
#include "ModbusService.h"
#include "Data.h"
#include "ActionOutput.h"

namespace MonitoringComponents {
	class Action {
	public:
		Action() {	}

		Action(ActionConfiguration configuration):address(configuration.modbusAddress) {
			this->actionId = configuration.actionId;
			this->actionType = configuration.actionType;
			this->output1 = nullptr;
			this->output2 = nullptr;
			this->output3 = nullptr;
		}

		int Id() {
			return this->actionId;
		}

		ActionType GetActionType() {
			return this->actionType;
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
			ModbusService::Update(this->address, true);
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
			ModbusService::Update(this->address, false);
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
		ModbusAddress address;
		ActionOutput* output1;
		ActionOutput* output2;
		ActionOutput* output3;
	};
};
