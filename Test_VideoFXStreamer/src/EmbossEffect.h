#pragma once

#include "BaseEffect.h"

class EmbossEffect : public BaseEffect {
public:
    
    float intensity;
    float colorization;
    float flowEffect;
    
    EmbossEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/emboss.frag";
        parseShader();
        name = "emboss";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "EMBOSS EFFECT", &enabled );
//        addWidget( gui->addSlider( "INTENSITY", 1.0, 100.0, &intensity ) );
        //        addWidget( gui->addSlider( "COLORIZATION", 0.0, 1.0, &colorization ) );
        settings = new ofxUISuperCanvas( "EMBOSS EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->addSlider( "INTENSITY", 1.0, 100.0, &intensity );
        settings->addSlider( "COLORIZATION", 0.0, 1.0, &colorization );
        settings->addSlider( "FLOW EFFECT", 0.0, 100.0, &flowEffect );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
    }
    
    void setCustomUniforms() {
        shader.setUniform1f("intensity", intensity );
        shader.setUniform1f("colorization", colorization );
        shader.setUniform1f( "flowEffect", flowEffect );
    }
};