#pragma once

#include "BaseEffect.h"
#include "ofxBox2d.h"
#include "ofxFluid.h"

class PhysBallEffect : public BaseEffect {
public:
    
    ofxFluid fluid;
    ofxBox2d box2d;
    bool isInited;
    vector<ofxBox2dCircle> circles;
    ofxUIVec3f gravity;
    float forceScale;
    float numBalls = 200;
    bool drawFluid;
    float fluidDissipation = 0.99;
    
    PhysBallEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        name = "physball";
    }
    
    void allocate( int width, int height ) {
        cout << "phys ball allocate" << endl;
        ofxFXObject::allocate( width, height );
        fluid.allocate( width, height, .25 );
        fluid.dissipation = 0.99;
        fluid.velocityDissipation = fluidDissipation;
        fluid.setGravity(ofVec2f(0.0,0.0));
//        fluid.addConstantForce(ofPoint(width*0.5,height*0.85), ofPoint(0,-2), ofFloatColor(0.5,0.1,0.0), 10.f);
    }
    
    void initPhysics() {
        
        box2d.init();
        box2d.setFPS( 30.0 );
        box2d.setGravity( 0, 0 );
        box2d.registerGrabbing();
        box2d.createBounds(0,0,this->getWidth(),this->getHeight());
        // create some test circles
        for ( int i=0; i<numBalls; i++ ) {
            addRandomBall();
        }
        
        
        isInited = true;
    }
    
    void addRandomBall() {
        ofxBox2dCircle circle;
        circle.setPhysics(3.0, 0.53, 0.1);
        float radius = ofRandom( 16., 32. );
        circle.setup(box2d.getWorld(), ofRandom( radius, this->getWidth()-radius ), ofRandom( radius, this->getHeight()-radius ), radius );
        circles.push_back(circle);
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "PHYS BALL EFFECT", &enabled );
        
        settings = new ofxUISuperCanvas( "PHYS BALL EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->addSlider( "FORCE SCALE", 1.0, 100.0, &forceScale );
        settings->addSlider( "NUM BALLS", 1.0, 200.0, &numBalls );
        settings->addSlider( "FLUID DISSIPATION", 0.0, 1.0, &fluidDissipation );
        settings->addToggle( "DRAW FLUID", &drawFluid );
//        settings->add2DPad( "GRAVITY", ofxUIVec3f(-10,10,0), ofxUIVec3f(-10,10,0), &gravity );
        
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
                                        
    }
    
    void update() {
        
        if ( !isInited )
            initPhysics();
        
        
        fluid.dissipation = fluidDissipation;
        fluid.update();
        
        ofPushStyle();
        ofSetColor( 255, 255 );
        pingPong.dst->begin();
        ofClear(0);
        
        glEnable( diffuseTexture.textureTarget );
        glBindTexture( diffuseTexture.textureTarget, diffuseTexture.textureID );
        renderFrame();
        glDisable( diffuseTexture.textureTarget );
        
        while ( circles.size() < numBalls )
            addRandomBall();
        while ( circles.size() > numBalls ) {
            (circles.end()-1)->destroy();
            circles.erase( circles.end()-1 );
        }
        
        
        box2d.setGravity( gravity.x, gravity.y );
        box2d.update();
        if ( amount > 0.0 && farneback->getWidth() != 0 && farneback->getHeight() != 0 ) {
            for ( int i=0; i<circles.size(); i++ ) {
                float scale = farneback->getWidth() / (float)this->getWidth();
                ofxBox2dCircle &circle = circles[i];
                float radius = circle.getRadius() * scale;
                // get force amt for circle
                ofVec2f pos = circle.getPosition() * scale;
                ofRectangle boundingRect = ofRectangle( pos.x - radius, pos.y - radius, radius * 2.0, radius * 2.0 );
                ofVec2f flow = farneback->getAverageFlowInRegion( boundingRect );
                
                circle.addForce( flow, forceScale );
                
                fluid.addTemporalForce( circle.getPosition(), circle.getVelocity(), ofColor::white, 4.0 );
                
//                if ( ofGetMousePressed() )
                if ( !drawFluid )
                    ofCircle( circle.getPosition(), circle.getRadius() * amount );
//                else
//                    circle.draw();
            }
        }
        
        
        if ( drawFluid ) {
            fluid.draw( 0, 0, this->getWidth(), this->getHeight() );
//            fluid.getTextureReference().draw( 0, 0, this->getWidth(), this->getHeight() );
        }
        
        pingPong.dst->end();
        ofPopStyle();
    }
};