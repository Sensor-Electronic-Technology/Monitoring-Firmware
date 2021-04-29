#pragma once
#include <ArduinoSTL.h>
#include "Ref.h"

using namespace std;

namespace MonitoringComponents {


	class MonitoringComponent {
	public:
		MonitoringComponent(Ref<MonitoringComponent> parent = nullptr) : _parent(parent), _isLooping(false) {}
		void loop() {
			_isLooping = true;
			loopChildren();
			if (_isLooping) {
				privateLoop();
			}
		}
	protected:

		void SkipLoop() {
			_isLooping = false;
			if (*_parent != nullptr) _parent->SkipLoop();
		}

		void RegisterChild(Ref<MonitoringComponent> child) {
			_children.push_back(child);
		}

	private:
		Ref<MonitoringComponent> _parent;
		std::vector<Ref<MonitoringComponent>> _children;
		bool _isLooping;

		void loopChildren() {
			for (int i = 0; i < _children.size(); i++) {
				_children[i]->loop();
				if (!_isLooping) {
					SkipLoop();
					return;
				}
			}
		}

		virtual void privateLoop() = 0;
		
	};
};
