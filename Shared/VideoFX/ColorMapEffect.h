#pragma once

#include "BaseEffect.h"

class ColorMapEffect : public BaseEffect {
public:
    
    ofTextureData ramp;
    void setRamp( ofTextureData & ramp ) {
        this->ramp = ramp;
    }
    
    ColorMapEffect() {
        passes = 1;
        internalFormat = GL_RGBA;
        pathToShader = "shaders/colormap.frag";
        parseShader();
        name = "colormap";
    }
    
    void setupGUI( ofxUISuperCanvas *gui ) {
        gui->addSpacer();
        gui->addToggle( "COLOR MAP EFFECT", &enabled );
        settings = new ofxUISuperCanvas( "COLOR MAP EFFECT", ofRandom(ofGetWidth()), 0, 200, 200 );
        settings->setColorBack( ofColor( ofColor::yellow, 150 ) );
        settings->addSlider( "AMOUNT", 0.0, 1.0, &amount );
        settings->autoSizeToFitWidgets();
        settings->loadSettings( "GUI/effects/" + name + ".xml" );
    }
    
    void setCustomUniforms() {
        shader.setUniformTexture( "colorMap", ramp.textureTarget, ramp.textureID, 1 );
        shader.setUniform2f( "colorMapSize", ramp.tex_w, ramp.tex_h );
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