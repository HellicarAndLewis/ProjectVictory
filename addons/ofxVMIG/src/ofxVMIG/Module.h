#pragma once

namespace ofxVMIG {
    
    class Module {
    public:
        
        virtual void draw(float width, float height) = 0;
        
        // Called when module needs to populate an ofxUICanvas
        virtual void populateInspector(ofxUICanvas &canvas) = 0;
        
        // Called when a value is chaning in the inspector
        virtual void inspectorCallback(ofxUIEventArgs &args) = 0;
        
    };
    
}