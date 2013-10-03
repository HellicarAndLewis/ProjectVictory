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
};