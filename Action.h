#pragma once
#include <ArduinoSTL.h>
#include "ActionOutput.h"

class Action {
public:
	Action(){}

	Action(ActionConfiguration configuration){  
		
	}
	
	int Id() {
		return this->actionId;
	}

	void Invoke();
private:
	int actionId;
	OutputType type;
	State initialState;
	
	ActionConfiguration configuration;
	ActionOutput output1;
	ActionOutput output2;
	ActionOutput output3;

};