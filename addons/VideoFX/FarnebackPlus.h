#pragma once

#include "ofMain.h"
#include "ofxCv.h"
//#include "ofxFluid.h"

using namespace ofxCv;

class FarnebackPlus : public FlowFarneback {
public:
    
    ofFloatImage vf;
    
    void updateVectorFieldTexture();
//    void addForceToFluid( ofxFluid * fluid );
    
};
