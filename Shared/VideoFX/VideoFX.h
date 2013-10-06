#ifndef _VIDEO_FX
#define _VIDEO_FX

#include "ofMain.h"
#include "ofxUI.h"
#include "BaseEffect.h"
#include "FarnebackPlus.h"
#include "CircularTexture.h"
#include "ScanlinesEffect.h"
#include "KhronosEffect.h"
#include "BadTVEffect.h"
#include "RGBShiftEffect.h"
//#include "ofxFluid.h"
#include "OldVideoEffect.h"
#include "EmbossEffect.h"
#include "SharpenEffect.h"
#include "ColorMapEffect.h"
#include "FlowLinesEffect.h"
#include "PhysBallEffect.h"
#include "GridDistortEffect.h"


/*
    VideoFX is a manager class that owns all the different video effects
 */

class VideoFX {
public:
    
    ofColor color;
    void setColor( ofColor color ) {
        this->color.set( color );
        for ( int i=0; i<effects.size(); i++ ) {
            effects[i]->setColor( color );
        }
    }
    
    static int count;
    int num;
    
    bool opticalFlowEnabled;
    
    // current video source
    ofBaseImage *videoSource;
    // an image to store a downsampled version of the curent frame
    ofImage downsampledFrame;
    // does optical flow and provides a floating point vector field texture for use in shaders
    FarnebackPlus farneback;
    // a circular texture buffer that successive frames from the video source are read into
    CircularTexture circularTexture;
    // some fluid
    ofxFluid fluid;
    
    ofImage ramp;
    
    
    KhronosEffect khronosEffect;
    OldVideoEffect oldVideoEffect;
    BadTVEffect badTVEffect;
    ScanlinesEffect scanlinesEffect;
    RGBShiftEffect  rgbShiftEffect;
    EmbossEffect embossEffect;
    SharpenEffect sharpenEffect;
    ColorMapEffect colorMapEffect;
    FlowLinesEffect flowLinesEffect;
    PhysBallEffect physBallEffect;
    GridDistortEffect gridDistortEffect;
    // vector contains points to all effects
    vector<BaseEffect*> effects;
    // a pointer to the final effect of the chain. this is what gets drawn back to the screen
    BaseEffect *finalEffect;
    // gui
    ofxUISuperCanvas *vfxGUI;
    ofxUICanvas *presetGUI;
    ofxUIDropDownList *presetWidget;
    
    
    void presetEvent( ofxUIEventArgs &e );
    void guiEvent(ofxUIEventArgs &e);
    void updateGUI();
    void hideGUI();
    void showGUI();
    void setVideoSource( ofBaseImage * source );
    void init();
    void update( bool isFrameNew );
    void updateEffect( BaseEffect * effect );
    void draw( float posX, float posY, float width, float height );
    void reloadShaders();
    void fillPresets();
    void exit();
    
    void setOpticalFlowEnabled( bool enabled );
    
    
};


bool CompareBaseEffects( BaseEffect * a, BaseEffect * b );

#endif