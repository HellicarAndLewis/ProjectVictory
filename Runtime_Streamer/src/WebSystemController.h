#pragma once
#include "ofMain.h"
#include "WebSystem.h"
#include "ofxUI.h"
class VideoFX;
class Overlay;
class StreamerGUI;

typedef map<float, string> ScreenShotTriggers;
typedef map<float, string>::iterator ScreenShotTriggersIt;

#define MAX_IMAGES_PER_MINUTE 4.0f

// This is the example glue that connects the VideoFX to the WebSystem, and demo's some of the websystem utils.
// It is not really to be used directly. For example, having to vfx's channels has not been considered (see: `decayVideoFXToDefault` comments)

class WebSystemController {
public:
    WebSystemController()
    :videoFX(0),
    overlay(0),
    decayRate(0.3f),
    effectImpact(1.0f),
    webSystemIsEnabled(false),
    shoutoutsAreEnabled(false),
    countHashTags(false),
    commandsAreEnabled(false),
    shouldDecaysEffects(false),
    shouldTriggerFakeScreenshot(false),
    gui(0),
    lastScreenshotSavedAt(0.0f)
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
    void setGui(StreamerGUI *g) {
        gui = g;
    }
    
 public:
    void setEnabled(bool f);
    void setShoutOutsEnabled(bool f);
    void setCommandsEnabled(bool f);
    void setShouldDecayEffects(bool f);
    void setCountHashTags(bool f);
    void setDecayRate(float f);
    void setEffectImpact(float f);
    void triggerFakeScreenGrab(); /* this will make sure that "getNextScreenShotFilename()" returns a value so you can test it */

 public:
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

protected:
    
    ScreenShotTriggers screenShotTriggers;
    
    VideoFX *videoFX;
    Overlay *overlay;
    StreamerGUI *gui;
    
    // The precentage of decay per second (e.g. 0.1 == 10%)
    float decayRate;
    float effectImpact;

    float lastScreenshotSavedAt;
    
    bool shouldTriggerFakeScreenshot; /* used while testing, calling triggerFakeScreenGrab() will set this to true. */
    
    // Command overlay images
    
private:
    // State
    bool webSystemIsEnabled;
    bool shoutoutsAreEnabled;
    bool commandsAreEnabled;
    bool shouldDecaysEffects;
    bool countHashTags;
};

inline void WebSystemController::setEnabled(bool f) {
  webSystemIsEnabled = f;
}

inline void WebSystemController::setShoutOutsEnabled(bool f) {
  shoutoutsAreEnabled = f;
}

inline void WebSystemController::setCommandsEnabled(bool f) {
  commandsAreEnabled = f;
}

inline void WebSystemController::setShouldDecayEffects(bool f) {
  shouldDecaysEffects = f;
}

inline void WebSystemController::setCountHashTags(bool f) {
  countHashTags = f;
}

inline void WebSystemController::setDecayRate(float f) {
  decayRate = f;
}

inline void WebSystemController::setEffectImpact(float f) {
  effectImpact = f;
}

inline void WebSystemController::triggerFakeScreenGrab() {
  shouldTriggerFakeScreenshot = true;
}
