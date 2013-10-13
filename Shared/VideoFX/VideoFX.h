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
    
    KhronosEffect khronosEffect;
    BadTVEffect badTVEffect;
    ScanlinesEffect scanlinesEffect;
    RGBShiftEffect  rgbShiftEffect;
    ColorMapEffect colorMapEffect;
    FlowLinesEffect flowLinesEffect;
    GridDistortEffect gridDistortEffect;

    vector<BaseEffect*> effects;    // vector contains points to all effects

    BaseEffect *finalEffect;     // a pointer to the final effect of the chain. this is what gets drawn back to the screen

    // gui
    ofxUISuperCanvas *vfxGUI;
    ofxUICanvas *presetGUI;
    ofxUIDropDownList *presetWidget;
    
    void presetEvent( ofxUIEventArgs &e );
    void guiEvent(ofxUIEventArgs &e);
    void updateGUI();
    void hideGUI();
    void showGUI();
    void disableGuiEvents();
    void drawGUI();
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

inline void VideoFX::disableGuiEvents() {
#if 0  
  vfxGUI->disableAppEventCallbacks();
  presetGUI->disableAppEventCallbacks();
#else
  vfxGUI->disableAppDrawCallback();
  presetGUI->disableAppDrawCallback();
#endif  
  for(std::vector<BaseEffect*>::iterator it = effects.begin(); it != effects.end(); ++it) {
    BaseEffect* fx = *it;
    fx->disableGuiEvents();
  }
}

inline void VideoFX::drawGUI() {
  vfxGUI->draw();
  presetGUI->draw();

  for(std::vector<BaseEffect*>::iterator it = effects.begin(); it != effects.end(); ++it) {
    BaseEffect* fx = *it;
    fx->draw();
  }

}

bool CompareBaseEffects( BaseEffect * a, BaseEffect * b );

#endif
