#pragma once

#include "BaseEffect.h"

class FlowLinesEffect : public BaseEffect {
public:
    float stepSize = 8.0;
    float lineScale = 10.0;
    float threshold = .1;
    
    FlowLinesEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        name = "flowlines";
    }

    void setStepSize(float v) {
      stepSize = v;
    }

    void setLineScale(float v) {
      lineScale = v;
    }
    
    void setThreshold(float v) {
      threshold = v;
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
      /*
        gui->addSpacer();
        gui->addToggle( "FLOW LINES EFFECT", &enabled );
        
        settings = new ofxUISuperCanvas( "FLOW LINES EFFECT", ofRandom( ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->addSlider( "STEP SIZE", 2.0, 20.0, &stepSize );
        settings->addSlider( "LINE SCALE", 1.0, 20.0, &lineScale );
        settings->addSlider( "THRESHOLD", .1, 2.0, &threshold );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
      */
    }
    
    void update() {
        ofPushStyle();
        ofSetColor( 255, 255 );
        pingPong.dst->begin();
        ofClear(0);
        
        glEnable( diffuseTexture.textureTarget );
        glBindTexture( diffuseTexture.textureTarget, diffuseTexture.textureID );
        renderFrame();
        glDisable( diffuseTexture.textureTarget );
        
        if ( amount > 0.0 && farneback->getWidth() !=0 && farneback->getHeight() != 0 ) {
            
            // draw flow lines
            int cols = this->getWidth();
            int rows = this->getHeight();
            float xScale = farneback->getWidth() / (float)cols;
            float yScale = farneback->getHeight() / (float)rows;
            ofVec2f scale( xScale, yScale );
            ofEnableBlendMode( OF_BLENDMODE_ADD );
            
            float minScale = sqrt( threshold );
            for ( int x=0; x<cols; x+=stepSize ) {
                for ( int y=0; y<rows; y+=stepSize ) {
                    ofVec2f farnCoord( (int)(x*xScale), (int)(y*yScale) );
                    ofVec2f vel = (farneback->getFlowPosition( farnCoord.x, farnCoord.y ) - farnCoord);
                    if ( vel.lengthSquared() > threshold ) {
                        float angle = vel.angle( ofVec2f( 1.0, 0.0 ) );
                        float length = vel.length();
                        float hue, saturation, brightness;
                        color.getHsb( hue, saturation, brightness );
                        hue = hue > 255 ? hue - 255 : hue < 0 ? hue + 255 : hue;
                        saturation = ofMap( length, minScale, 20.0, 180, 255, true );
                        brightness = ofMap( length, minScale, 20.0, 100, 200, true );
                        ofColor col;
                        col.setHsb( hue, saturation , brightness );
                        
                        ofSetColor( col );
                        ofLine( x, y, x + vel.x * lineScale, y + vel.y * lineScale * amount );
                    }
                }
            }
        }
        
        pingPong.dst->end();
        ofPopStyle();
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
