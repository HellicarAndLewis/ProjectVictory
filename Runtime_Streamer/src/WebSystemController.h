#pragma once
#include "ofMain.h"
#include "WebSystem.h"

class VideoFX;
class Overlay;

// This is the example glue that connects the VideoFX to the WebSystem, and demo's some of the websystem utils.
// It is not really to be used directly. For example, having to vfx's channels has not been considered (see: `decayVideoFXToDefault` comments)

class WebSystemController {
public:
    WebSystemController()
    :videoFX(0),
    overlay(0)
    {};
    
    // Config
    static const int UPDATE_VOTES_EVERY_MS = 300;
    
    // Lifecycle
    void init();
    void update();
    
    WebSystem::Connection connection;
    
    // Web system events
    void onHashTagCount(Json::Value body);
    void onShoutout(Json::Value body);
    void onCommand(Json::Value body);
    
    // Vide0 effects
    void setVideoFX( VideoFX *vfx ) {
        videoFX = vfx;
    }
    
    // Overlay
    void setOverlay( Overlay *overlay) {
        this->overlay = overlay;
    }
    
    void decayVideoFXToDefault();
    
private:
    
    VideoFX *videoFX;
    Overlay *overlay;
    
};