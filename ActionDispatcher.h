#pragma once
#include <ArduinoSTL.h>
#include <map>
#include "Configuration.h"
#include "Data.h"

//class ActionDispatcher{
//public:
//	ActionDispatcher(){}
//
//	static ActionDispatcher* Instance() {
//		if (instance == nullptr) {
//			instance = new ActionDispatcher;
//		}
//		return instance;
//	}
//
//	static void InvokeAction(int actionId, ChannelAddress caller) {
//		auto instance = Instance();
//		auto it=std::find_if(instance->actions.begin(), instance->actions.end(), [&](const ActionConfiguration& action) {
//				return action.actionId == actionId;
//		});
//		if (it != instance->actions.end()) {
//			if (it->type == OutputType::NCHO || it->type == OutputType::NOHC) {
//				//perform action and move on
//			} else {
//				//stor actions that need to me cleared
//				Registrations* registrations = instance->actionMap[actionId];
//				if (registrations == nullptr) {
//					registrations = new Registrations;
//					instance->actionMap[actionId] = registrations;
//				}
//
//			}
//		}
//	}
//
//	static void ClearAction(int actionId, ChannelAddress caller, OutputType outType) {
//
//	}
//
//
//private:
//	static ActionDispatcher* instance;
//	typedef std::vector<ChannelAddress> Registrations;
//	//std::vector<OutputChannel*> outputChannels;
//	std::map<int, Registrations*> actionMap;
//	std::vector<ActionConfiguration> actions;
//
//	static 
//};

