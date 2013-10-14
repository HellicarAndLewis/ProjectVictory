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

    ofBaseImage *videoSource;  // current video source
    ofImage downsampledFrame;  // an image to store a downsampled version of the curent frame
    FarnebackPlus farneback;    // does optical flow and provides a floating point vector field texture for use in shaders

    CircularTexture circularTexture;  // a circular texture buffer that successive frames from the video source are read into
    ofxFluid fluid;     // some fluid
    ofImage ramp;

    /* roxlu: these are accessed from testApp::updateGUI */
    KhronosEffect khronosEffect;
    BadTVEffect badTVEffect;
    ScanlinesEffect scanlinesEffect;
    RGBShiftEffect  rgbShiftEffect;
    ColorMapEffect colorMapEffect;
    FlowLinesEffect flowLinesEffect;
    GridDistortEffect gridDistortEffect;

    vector<BaseEffect*> effects;    // vector contains points to all effects

    BaseEffect *finalEffect;     // a pointer to the final effect of the chain. this is what gets drawn back to the screen

    // new gui
    void setKhronosEnabled(bool f);
    void setBadTVEnabled(bool f);
    void setScanLinesEnabled(bool f);
    void setRGBShiftEnabled(bool f);
    void setColorMapEnabled(bool f);
    void setFlowLinesEnabled(bool f);
    void setGridDistortEnabled(bool f);
    
    // void presetEvent( ofxUIEventArgs &e );
    //void guiEvent(ofxUIEventArgs &e);
    //    void updateGUI();
    //    void hideGUI();
    //    void showGUI();

    bool setVideoSource( ofBaseImage * source );
    void init();
    void update( bool isFrameNew );
    void updateEffect( BaseEffect * effect );
    void draw( float posX, float posY, float width, float height );
    void reloadShaders();
    //  void fillPresets();
    void exit();
    
    void setOpticalFlowEnabled( bool enabled );
    
};

// GUI
// ------------------------------------------------
inline void VideoFX::setKhronosEnabled(bool f) {
  khronosEffect.enabled = f;
}

inline void VideoFX::setBadTVEnabled(bool f) {
  badTVEffect.enabled = f;
}

inline void VideoFX::setScanLinesEnabled(bool f) {
  scanlinesEffect.enabled = f;
}

inline void VideoFX::setRGBShiftEnabled(bool f) {
  rgbShiftEffect.enabled = f;
}

inline void VideoFX::setColorMapEnabled(bool f) {
  colorMapEffect.enabled = f;
}

inline void VideoFX::setFlowLinesEnabled(bool f) {
  flowLinesEffect.enabled = f;
}

inline void VideoFX::setGridDistortEnabled(bool f) {
  gridDistortEffect.enabled = f;
}

bool CompareBaseEffects( BaseEffect * a, BaseEffect * b );

#endif
