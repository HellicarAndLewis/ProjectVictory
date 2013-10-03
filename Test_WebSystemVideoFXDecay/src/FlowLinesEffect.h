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
    
    void setupGUI( ofxUISuperCanvas *gui ) {
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
            for ( int x=0; x<cols; x+=stepSize ) {
                for ( int y=0; y<rows; y+=stepSize ) {
                    ofVec2f farnCoord( (int)(x*xScale), (int)(y*yScale) );
                    ofVec2f vel = (farneback->getFlowPosition( farnCoord.x, farnCoord.y ) - farnCoord);
                    if ( vel.lengthSquared() > threshold ) {
                        float angle = vel.angle( ofVec2f( 1.0, 0.0 ) );
                        ofColor col;
//                        col.setHsb( ofMap(angle, -180, 180, 0, 255 ), ofMap(vel.length(), 0.0, 10.0, 0, 255 ) , 128 );
                        col.setHsb( ofMap(angle, -180, 180, 0, 255 ), 255 , 128 );
                        ofSetColor( col );
                        ofLine( x, y, x + vel.x * lineScale, y + vel.y * lineScale * amount );
                    }
                }
            }
        }
        
        pingPong.dst->end();
        ofPopStyle();
    }
};