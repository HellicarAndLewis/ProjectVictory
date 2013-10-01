#pragma once
#include "ofxCvGui/Panels/Base.h"
#include "ofxUICanvas.h"
#include "Module.h"

namespace ofxVMIG {
	class Inspector : public ofxCvGui::Panels::Base {
	public:
		Inspector();
		void setFocus(Module&);
		void clearFocus();
	protected:
		void mouse(ofxCvGui::MouseArguments&);
		void boundsChange(ofxCvGui::BoundsChangeArguments&);
		void onUIChange(ofxUIEventArgs&);
		ofxUICanvas widgets;
		Module* focused;
	};
}