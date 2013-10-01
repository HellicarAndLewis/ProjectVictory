#pragma once
#include "ofxUI.h"
#include <memory>

namespace ofxVMIG {
    class Module {
    public:
		virtual void setup() { }
		virtual void update() { };
		virtual void draw(float width, float height) { };
        
        // Called when module needs to populate an ofxUICanvas
		virtual void populateInspector(ofxUICanvas &canvas) { };
        
        // Called when a value is chaning in the inspector
		virtual void inspectorCallback(ofxUIEventArgs &args) { };
    };

	typedef std::shared_ptr<Module> ModulePtr;
}