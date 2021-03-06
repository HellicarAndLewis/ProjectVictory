#pragma once
#include "ofMain.h"
#include "WebSystem.h"
#include "ofxUI.h"
class VideoFX;
class Overlay;

typedef map<float, string> ScreenShotTriggers;
typedef map<float, string>::iterator ScreenShotTriggersIt;

// This is the example glue that connects the VideoFX to the WebSystem, and demo's some of the websystem utils.
// It is not really to be used directly. For example, having to vfx's channels has not been considered (see: `decayVideoFXToDefault` comments)

class WebSystemController {
public:
    WebSystemController()
    :videoFX(0),
    overlay(0),
    webSystemIsEnabled(false),
    shoutoutsAreEnabled(false),
    commandsAreEnabled(false),
    shouldDecaysEffects(false)
    {};
    
    // Config
    static const int UPDATE_VOTES_EVERY_MS = 300;
    static const float DECAY_EFFECT_EVERY_SEC = 1.0f/15.0f;
    
    // Lifecycle
    void init();
    void update();
    
    // Web system
    WebSystem::Connection connection;
    ofxUISuperCanvas *websystemGUI;
    void initWebSystemGUI();
    // State
    bool webSystemIsEnabled;
    bool shoutoutsAreEnabled;
    bool commandsAreEnabled;
    bool shouldDecaysEffects;
    
    // Web system events
    void onHashTagCount(Json::Value body);
    void onShoutout(Json::Value body);
    void onCommand(Json::Value body);
    
    // Video effects
    void setVideoFX( VideoFX *vfx ) {
        videoFX = vfx;
    }
    
    // Overlay
    void setOverlay( Overlay *overlay) {
        this->overlay = overlay;
    }
    
    void decayVideoFXToDefault();
    
    // Image saving
    string getNextScreenShotFilename();
    
    // VoteSystem
    void initVotingGUI();
    ofxUISuperCanvas *voteGUI;
    ofxUITextInput *vote1TextInput;
    ofxUITextInput *vote2TextInput;
    void voteingGUIEvent(ofxUIEventArgs &e);
    
protected:
    
    ScreenShotTriggers screenShotTriggers;
    
    VideoFX *videoFX;
    Overlay *overlay;
    
};