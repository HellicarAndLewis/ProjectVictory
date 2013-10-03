#pragma once
#include "BaseEffect.h"

class SharpenEffect : public BaseEffect {
public:
    
    float weirdness;
    float theMix;
    
    SharpenEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/sharpen.frag";
        parseShader();
        name = "sharpen";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "SHARPEN EFFECT", &enabled );
        settings = new ofxUISuperCanvas( "SHARPEN EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "amount", 0.0, 1.0, &amount );
        settings->addSlider("sharpen mix", 0.0, 1.0, &theMix );
        settings->addSlider("weirdness", 0.0, 1.0, &weirdness );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
    }
    
    void setCustomUniforms() {
        shader.setUniform1f( "weirdness", weirdness );
        shader.setUniform1f( "theMix", theMix );
    }
    
    ShaderMap getShaderMap() {
        ShaderMap shaderMap;
        shaderMap["weirdness"] = weirdness;
        shaderMap["theMix"] = theMix;
        return shaderMap;
    }
    
    
};