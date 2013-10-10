#pragma once

#include "ofMain.h"
#include "BaseEffect.h"

class RGBShiftEffect : public BaseEffect {
public:
    
    float angle;
    float rgbAmount;
    
    RGBShiftEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/rgbshift.frag";
        parseShader();
        name = "rgbshift";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "RGB SHIFT EFFECT", &enabled );
//        addWidget( gui->addSlider( "RGB AMOUNT", 0.0, 0.1, &amount ) );
        //        addWidget( gui->addSlider( "RGB ANGLE", 0.0, 2.0, &angle ) );
        
        settings = new ofxUISuperCanvas( "RGB SHIFT EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->addSlider( "RGB AMOUNT", 0.0, 0.1, &rgbAmount );
        settings->addSlider( "RGB ANGLE", 0.0, 2.0, &angle );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
    }
    
    void setCustomUniforms() {
        shader.setUniform1f( "amount", amount );
        shader.setUniform1f( "rgbAmount", rgbAmount );
        shader.setUniform1f( "angle", angle );
    }
    
    ShaderMap getShaderMap() {
        ShaderMap shaderMap;
        shaderMap["amount"] = amount;
        return shaderMap;
    }
    
    ShaderMap getShaderDefaultMap() {
        ShaderMap shaderMap;
        shaderMap["amount"] = 0.0f;
        return shaderMap;
    }
    
    void applyShaderMap(ShaderMap shaderMap) {
        if (shaderMap.isMember("amount"))        { amount = shaderMap["amount"].asDouble(); }
    }
};