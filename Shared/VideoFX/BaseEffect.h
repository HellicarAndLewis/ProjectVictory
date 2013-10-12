#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"
#include "ofxUI.h"
#include "FarnebackPlus.h"
#include "json.h"

typedef Json::Value ShaderMap;

class BaseEffect : public ofxFXObject {

public:
    string name;    
    bool enabled = false;
    
    ofxUISuperCanvas *settings;
    
    FarnebackPlus *farneback;
    
    vector<ofxUIWidget*> widgets;
    void setWidgetVisibility( bool visible ) {
        for ( int i=0; i<widgets.size(); i++ ) {
            widgets[i]->setVisible( visible );
        }
    }

    #if 1
    void disableGuiEvents() {
      /*
      for(std::vector<ofxUIWidget*>::iterator it = widgets.begin(); it != widgets.end(); ++it) {
        (*it)->disableAppDrawCallback();
      }
      */
      //settings->disableAppEventCallbacks();
      settings->disableAppDrawCallback();
    }
    #endif
    
    string getPathToShader() {
        return pathToShader;
    }
    
    void addWidget( ofxUIWidget *widget ) {
        widgets.push_back( widget );
    }
    
    // use this for hot-loading shader while app is running
    void reloadShader() {
        parseShader();
        compileCode();
    }
    
    float amount;
    void setAmount( float amount ) {
        this->amount = amount;
    }
    
    ofColor color;
    virtual void setColor( ofColor color ) {
        this->color.set( color );
    }
    
    ofTextureData diffuseTexture;
    void setDiffuseTexture( ofTextureData & diffuseTexture ) {
        this->diffuseTexture = diffuseTexture;
    }
    
    ofTextureData vectorField;
    void setVectorField( ofTextureData & vectorField ) {
        this->vectorField = vectorField;
    }
    
    virtual void setupGUI( ofxUISuperCanvas *gui ) {
        
    }
    
    virtual void setCustomUniforms() {
        
    }
    
    void saveSettings( string dir ) {
        settings->saveSettings( dir + name + ".xml" );
    }
    
    void loadSettings( string dir ) {
        
        // check if it exists
        ofFile file( dir + name + ".xml" );
        if ( file.exists() )
            settings->loadSettings( dir + name + ".xml" );
        else
            enabled = false;
    }
    
    void update() {
        ofPushStyle();
        ofSetColor(255,255);
        pingPong.dst->begin();
        ofClear(0);
        shader.begin();
        shader.setUniformTexture("diffuseTexture", diffuseTexture.textureTarget, diffuseTexture.textureID, 0 );
        shader.setUniformTexture( "vectorField", vectorField.textureTarget, vectorField.textureID, 2 );
        shader.setUniform2f( "vectorFieldSize", vectorField.tex_w, vectorField.tex_h );
        shader.setUniform1f( "amount", amount );
        shader.setUniform1f("time", ofGetElapsedTimef() );
        shader.setUniform2f("size", (float)width, (float)height);
        shader.setUniform2f("resolution", (float)width, (float)height);
        shader.setUniform2f("mouse", (float)(ofGetMouseX()/width), (float)(ofGetMouseY()/height));
        setCustomUniforms();
        renderFrame();
        shader.end();
        pingPong.dst->end();
        ofPopStyle();
    }
    
    // For exporting the shader uniforms
    virtual ShaderMap getShaderMap() {
        ShaderMap shaderMap;
        return shaderMap;
    }
    
    // For defineing shader default values 
    virtual ShaderMap getShaderDefaultMap() {
        ShaderMap shaderMap;
        return shaderMap;
    }
    
    // For applying a shader map
    virtual void applyShaderMap(ShaderMap shaderMap) {
        
    }
    
protected:
    
    
    string pathToShader = "";
    
    void parseShader() {
        fragmentShader = "";
        ofBuffer buffer = ofBufferFromFile( pathToShader );
        if ( buffer.size() ) {
            while ( !buffer.isLastLine() )
                fragmentShader += buffer.getNextLine() + "\n";
//            cout << fragmentShader << endl;
        }
        else
            cout << "Effect shader source missing! " << pathToShader << endl;
    }
};
