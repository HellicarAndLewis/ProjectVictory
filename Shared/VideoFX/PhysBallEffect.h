#pragma once

#include "BaseEffect.h"
#include "ofxBox2d.h"
#include "ofxFluid.h"
#include "ofxNormals.h"

class PhysBallEffect : public BaseEffect {
public:
    
    ofxFluid fluid;
    ofxNormals normals;
    
    ofxBox2d box2d;
    bool isInited;
    vector<ofxBox2dCircle> circles;
    ofxUIVec3f gravity;
    float forceScale;
    float numBalls = 200;
    bool drawFluid;
    float fluidDissipation = 0.99;
    float radiusLow = 16;
    float radiusHigh = 32;
    float displacementScale = 10.0;
    
    PhysBallEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        name = "physball";
        pathToShader = "shaders/physball.frag";
        parseShader();
    }
    
    void allocate( int width, int height ) {
        ofxFXObject::allocate( width, height );
        fluid.allocate( width, height, .25 );
        fluid.dissipation = 0.99;
        fluid.velocityDissipation = fluidDissipation;
        fluid.setGravity(ofVec2f(0.0,0.0));
        
        normals.allocate( width / 4, height / 4 );
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
        float radius = ofRandom( radiusLow, radiusHigh );
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
        settings->addSlider( "RADIUS LOW", 8.0, 128.0, &radiusLow );
        settings->addSlider( "RADIUS HIGH", 8.0, 128.0, &radiusHigh );
        settings->addSlider( "FLUID DISSIPATION", 0.0, 1.0, &fluidDissipation );
        settings->addSlider( "FLUID DISPLACEMENT", 0.0, 100.0, &displacementScale );
        settings->addToggle( "DRAW FLUID", &drawFluid );
        //        settings->add2DPad( "GRAVITY", ofxUIVec3f(-10,10,0), ofxUIVec3f(-10,10,0), &gravity );
        
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
        ofAddListener( settings->newGUIEvent, this, &PhysBallEffect::guiEvent );
    }
    
    void guiEvent( ofxUIEventArgs &e ) {
        string name = e.widget->getName();
        if ( name == "RADIUS LOW" || name == "RADIUS HIGH" ) {
            for ( int i=0; i<circles.size(); i++ ) {
                circles[i].setRadius( ofRandom( radiusLow, radiusHigh ) );
            }
        }
    }
    
    void update() {
        
        if ( !isInited )
            initPhysics();
        
        
        fluid.dissipation = fluidDissipation;
        fluid.update();
        normals << fluid;
        normals.update();
        
        ofPushStyle();
        ofSetColor( 255, 255 );
        pingPong.dst->begin();
        ofClear(0);
        
        shader.begin();
        shader.setUniformTexture("diffuseTexture", diffuseTexture.textureTarget, diffuseTexture.textureID, 0 );
        shader.setUniformTexture( "vectorField", vectorField.textureTarget, vectorField.textureID, 2 );
        shader.setUniform2f( "vectorFieldSize", vectorField.tex_w, vectorField.tex_h );
        shader.setUniformTexture( "displacementTexture", normals.getTextureReference(), 1 );
        shader.setUniform2f( "displacementTextureSize", normals.getWidth(), normals.getHeight() );
        shader.setUniform1f( "displacementScale", displacementScale );
        
        shader.setUniform1f("time", ofGetElapsedTimef() );
        shader.setUniform2f("size", (float)width, (float)height);
        shader.setUniform2f("resolution", (float)width, (float)height);
        shader.setUniform2f("mouse", (float)(ofGetMouseX()/width), (float)(ofGetMouseY()/height));
        
        //        glEnable( diffuseTexture.textureTarget );
        //        glBindTexture( diffuseTexture.textureTarget, diffuseTexture.textureID );
        renderFrame();
        //        glDisable( diffuseTexture.textureTarget );
        
        shader.end();
        
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

                ofVec2f pos = circle.getPosition();
                if(pos.x != pos.x) {
#if !defined(NDEBUG)
                  printf("Circle.getPosition() returned an invalid position in PhysBalEffect.h\n");
#endif
                  continue;
                }

                // get force amt for circle
                pos *= scale;
                ofRectangle boundingRect = ofRectangle( pos.x - radius, pos.y - radius, radius * 2.0, radius * 2.0 );
                ofVec2f flow = farneback->getAverageFlowInRegion( boundingRect );
                
                circle.addForce( flow, forceScale );
                
                ofColor col;
                float hue = color.getHue();
                float brightness = 128, saturation = 255;
                hue += ofMap( sin( i * .1 ), -1.0, 1.0, -5.0, 5.0 );
                hue = hue > 255 ? hue - 255 : hue < 0 ? hue + 255 : hue;
                saturation = ofMap( circle.getVelocity().length(), 2.0, 10.0, 180, 255, true );
                brightness = ofMap( circle.getVelocity().length(), 2.0, 10.0, 100, 200, true );
                col.setHsb( hue, saturation, brightness );
                
                
                fluid.addTemporalForce( circle.getPosition(), circle.getVelocity(), col, circle.getRadius() / 8.0 );
                
//                if ( !drawFluid )
//                    ofCircle( circle.getPosition(), circle.getRadius() * amount );
            }
        }
        
        
        if ( drawFluid ) {
            //            fluid.draw( 0, 0, this->getWidth(), this->getHeight() );
            ofEnableBlendMode( OF_BLENDMODE_ADD );
            //            ofEnableBlendMode( OF_BLENDMODE_ALPHA );
            fluid.getTextureReference().draw( 0, 0, this->getWidth(), this->getHeight() );
            //            normals.getTextureReference().draw( 0, 0, this->getWidth(), this->getHeight() );
        }
        
        pingPong.dst->end();
        ofPopStyle();
    }
};
