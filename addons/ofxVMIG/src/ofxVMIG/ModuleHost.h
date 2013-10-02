#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "Module.h"

namespace ofxVMIG {
	class ModuleHost : public ofxCvGui::Panels::Base {
	public:
		ModuleHost(Module&);
	protected:
		Module& module;
	};
}