#pragma once

#include "ofMain.h"
#include "BaseEffect.h"

class KhronosEffect : public BaseEffect {
public:
    
    ofTextureData video3D;
    void setVideo3D( ofTextureData & video3D ) {
        this->video3D = video3D;
    }
    
    ofTextureData mask;
    void setMask( ofTextureData & mask ) {
        this->mask = mask;
    }
    
    float depthOffset = 0;
    void setDepthOffset( float depthOffset ) {
        this->depthOffset = depthOffset;
    }
    
    float magScaler = 1.0;
    void setMagScaler( float magScaler ) {
        this->magScaler = magScaler;
    }
    
    KhronosEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/khronos.frag";
        parseShader();
        name = "khronos";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "KHRONOS EFFECT", &enabled );
//        addWidget( gui->addSlider( "MAG SCALER", 1.0, 10.0, &magScaler ) );
        
        settings = new ofxUISuperCanvas( "KHRONOS EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "amount", 0.0, 1.0, &amount );
        settings->addSlider( "MAG SCALER", 1.0, 10.0, &magScaler );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/"+name+".xml" );
    }
    
    void update() {
        ofPushStyle();
        ofSetColor(255,255);
        
        pingPong.dst->begin();
        ofClear( 0 );
        
        
        shader.begin();
        shader.setUniformTexture( "mask", mask.textureTarget, mask.textureID, 0 );
        shader.setUniform2f( "maskSize", mask.tex_w, mask.tex_h );
        shader.setUniformTexture( "video3D", video3D.textureTarget, video3D.textureID, 1 );
        shader.setUniformTexture( "vectorField", vectorField.textureTarget, vectorField.textureID, 2 );
        shader.setUniform2f( "vectorFieldSize", vectorField.tex_w, vectorField.tex_h );
        
        shader.setUniform1f( "depthOffset", depthOffset );
        shader.setUniform1f( "magScaler", magScaler );
        
        shader.setUniform1f( "amount", amount );
        shader.setUniform1f("time", ofGetElapsedTimef() );
        shader.setUniform2f("size", (float)width, (float)height);
        shader.setUniform2f("resolution", (float)width, (float)height);
        shader.setUniform2f("mouse", (float)(ofGetMouseX()/width), (float)(ofGetMouseY()/height));
        
        
        renderFrame();

        shader.end();
        
        pingPong.dst->end();
        ofPopStyle();
    }
};