#pragma once

#include "BaseEffect.h"

class OldVideoEffect : public BaseEffect {
public:
    OldVideoEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/oldvideo.frag";
        parseShader();
        name = "oldvideo";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "OLD VIDEO EFFECT", &enabled );
        settings = new ofxUISuperCanvas( "OLD VIDEO EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
    }
    
    void setCustomUniforms() {
        
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
    
    void applyShaderMap(ShaderMap shaderMap, float multiplier) {
        if (shaderMap.isMember("amount"))        { amount = shaderMap["amount"].asDouble() * multiplier; }
    }
};