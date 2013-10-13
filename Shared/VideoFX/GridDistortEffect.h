#pragma once

#import "BaseEffect.h"
#import "MSAPhysics2D.h"

#define	SPRING_MIN_STRENGTH		0.005
#define SPRING_MAX_STRENGTH		0.1

using namespace msa::physics;

class GridDistortEffect : public BaseEffect {
public:
    
    ofMesh mesh;
    float stepSize = 8.;
    int xSteps, ySteps;
    float particleRadius = 8.;
    float forceScale = 1.0;
    float springStrength = .1;
    float drag = 1.0;
    
    bool showGrid = false;
    
    World2D physics;
    
    
    GridDistortEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        name = "griddistort";
        initMesh();
    }
    
    void initMesh() {
        
        mesh.clear();
        physics.clear();
        physics.setDrag( drag );
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        ySteps = (int)(getHeight() / stepSize);
        xSteps = (int)(getWidth() / stepSize);
        float xStepSize = getWidth() / (float)(xSteps-1);
        float yStepSize = getHeight() / (float)(ySteps-1);
        for(int y = 0; y < ySteps; y++) {
            for(int x = 0; x < xSteps; x++) {
                
                mesh.addVertex(ofVec2f(x * xStepSize, y * yStepSize));
                mesh.addTexCoord(ofVec2f(x * xStepSize, y * yStepSize));
                
                ofVec2f position(x * xStepSize, y * yStepSize);
                Particle2D *particle = physics.makeParticle( position );
                particle->moveBy( ofVec2f( ofRandom(-10.0,10.0), ofRandom(-10.0,10.0 ) ) );

                Particle2D *tether      = physics.makeParticle( ofVec2f( x * xStepSize, y * yStepSize ) );

                // tether is fixed
                tether->makeFixed();
                if ( x==0 || x==xSteps-1 || y==0 || y==ySteps-1 )
                    particle->makeFixed();
                else {
                    physics.makeSpring( particle, tether, springStrength, 0 );
                }
            }
        }
        for(int y = 0; y + 1 < ySteps; y++) {
            for(int x = 0; x + 1 < xSteps; x++) {
                int nw = y * xSteps + x;
                int ne = nw + 1;
                int sw = nw + xSteps;
                int se = sw + 1;
                mesh.addIndex(nw);
                mesh.addIndex(ne);
                mesh.addIndex(se);
                mesh.addIndex(nw);
                mesh.addIndex(se);
                mesh.addIndex(sw);
            }
        }
    }
    
    void setupGUI( ofxUISuperCanvas * gui ) {
        
        gui->addSpacer();
        gui->addToggle( "GRID DISTORT EFFECT", &enabled );
        
        settings = new ofxUISuperCanvas( "GRID DISTORT EFFECT", ofRandom( ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->addSlider( "STEP SIZE", 8.0, 32.0, &stepSize );
        settings->addSlider( "PARTICLE RADIUS", 2.0, 16.0, &particleRadius );
        settings->addSlider( "FORCE SCALE", 1.0, 100.0, &forceScale );
        settings->addSlider( "SPRING STRENGTH", .005, .2, &springStrength );
        settings->addSlider( "DRAG", 0.0, 1.0, &drag );
        settings->addToggle( "SHOW GRID", &showGrid );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
        ofAddListener( settings->newGUIEvent, this, &GridDistortEffect::guiEvent );
    }
    
    void guiEvent( ofxUIEventArgs &e ) {
        string name = e.widget->getName();
        if ( name == "SPRING STRENGTH" ) {
            for ( int i=0; i<physics.numberOfSprings(); i++ ) {
                Spring2D *spring = physics.getSpring(i);
                spring->setStrength( springStrength );
            }
        }
        else if ( name == "DRAG" ) {
            physics.setDrag( drag );
        }
    }
    
    void update() {

      if ( xSteps != (int)(getWidth() / stepSize) || ySteps != (int)(getHeight() / stepSize) ) {
            initMesh();
      }
        
        ofPushStyle();
        ofSetColor( 255, 255 );
        pingPong.dst->begin();
        ofClear(0);
        
        glEnable( diffuseTexture.textureTarget );
        glBindTexture( diffuseTexture.textureTarget, diffuseTexture.textureID );
//        renderFrame();
        mesh.draw();
        glDisable( diffuseTexture.textureTarget );
        
        if ( showGrid || ofGetMousePressed() ) {
            mesh.drawWireframe();
        }
        
        ofSetColor( ofColor::red );
        
        if ( farneback->getWidth() != 0 && farneback->getHeight() != 0 ) {
            float scale = farneback->getWidth() / (float)this->getWidth();
            float radius = particleRadius * scale;
            for ( int i=0; i<physics.numberOfParticles(); i++ ) {
                Particle2D *p = physics.getParticle(i);
                if ( p->isFixed() ) {
                    continue;
                }
                else {
                    Particle2D *tether = physics.getParticle(i+1);
                    // get force where this particle is
                    ofVec2f pos = p->getPosition();
                    printf(">>>> %f - %f\n", pos.x, pos.y);

                    if(pos.x != pos.x) {
                      printf("...- continue\n");
                      continue;
                    }

                    pos *= scale;


                    ofVec2f farnCoord( (int)ofClamp(pos.x,0,farneback->getWidth()-1), (int)ofClamp(pos.y,0,farneback->getHeight()-1) );

                    //ofVec2f ff = farneback->getFlowPosition( farnCoord.x, farnCoord.y );
                    //printf("farneback.w = %d, h = %d, farn: %f, %f, fff: %f, %f\n", farneback->getWidth(), farneback->getHeight(), farnCoord.x, farnCoord.y, ff.x, ff.y);
                    ofVec2f flow = (farneback->getFlowPosition( farnCoord.x, farnCoord.y ) - farnCoord );
                    if(flow.x != flow.x || flow.y != flow.y) {
                      printf("warning: the farneback values are invalid.\n");
                      continue;
                    }
                    //printf("%f, %f, %f\n", forceScale, flow.x, flow.y);
                    p->addVelocity( flow * forceScale );
                    mesh.setVertex( i/2, tether->getPosition().interpolated( p->getPosition(), amount ) );
                }
            }
            printf("update grid distort.\n");
            physics.update();
            //            ::exit(0);
        }
        printf("...\n");
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
    
    void applyShaderMap(ShaderMap shaderMap) {
        if (shaderMap.isMember("amount"))        { amount = shaderMap["amount"].asDouble(); }
    }
    
};

































