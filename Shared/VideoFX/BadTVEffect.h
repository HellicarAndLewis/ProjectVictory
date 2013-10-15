#pragma once

#include "ofMain.h"
#include "BaseEffect.h"

class BadTVEffect : public BaseEffect {
public:
    float speed;
    void setSpeed( float speed ) {
        this->speed = speed;
    }
    float rollSpeed;
    void setRollSpeed( float rollSpeed ) {
        this->rollSpeed = rollSpeed;
    }
    float distortion;
    void setDistortion( float distortion ) {
        this->distortion = distortion;
    }
    float distortion2;
    void setDistortion2( float distortion2 ) {
        this->distortion2 = distortion2;
    }
    
    BadTVEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/badtv.frag";
        parseShader();
        name = "badtv";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
      /*
        gui->addSpacer();
        gui->addToggle( "BAD TV EFFECT", &enabled );
        settings = new ofxUISuperCanvas( "BAD TV EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->addSlider( "THICK DISTORT", 0.1, 20.0, &distortion );
        settings->addSlider( "FINE DISTORT", 0.1, 20.0, &distortion2 );
        settings->addSlider( "DISTORT SPEED", 0.0, 1.0, &speed );
        settings->addSlider( "ROLL SPEED", 0.0, 1.0, &rollSpeed );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/"+name+".xml" );
      */
    }
    
    void setCustomUniforms() {
        shader.setUniform1f( "speed", speed );
        shader.setUniform1f( "rollSpeed", rollSpeed );
        shader.setUniform1f( "distortion", distortion );
        shader.setUniform1f( "distortion2", distortion2 );
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
    
    void applyShaderMap(ShaderMap shaderMap, float multiplier)  {
        if (shaderMap.isMember("amount"))        { amount = shaderMap["amount"].asDouble() * multiplier; }
    }


    // GUI 
    // -------------------------------------

    void setThickDistort(float v) {
      distortion = v;
    }

    void setFineDistort(float v) {
      distortion2 = v;
    }

    void setDistortSpeed(float v) {
      speed = v;
    }
};
