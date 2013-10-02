#include "ModuleHost.h"

namespace ofxVMIG {
	//----------
	ModuleHost::ModuleHost(Module& module) :
		module(module) {
		this->module.setup();
		this->onUpdate += [this] (ofxCvGui::UpdateArguments&) {
			this->module.update();
		};
		this->onDraw += [this] (ofxCvGui::DrawArguments&) {
			this->module.draw(this->getWidth(), this->getHeight());
		};
	}
}