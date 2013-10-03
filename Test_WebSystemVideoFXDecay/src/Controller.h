#pragma once
#include "ofMain.h"
#include "WebSystem.h"
#include "VideoFXExporter.h"
class VideoFX;

// This is the example glue that connects the VideoFX to the WebSystem, and demo's some of the websystem utils.
// It is not really to be used directly. For example, having to vfx's channels has not been considered (see: `decayVideoFXToDefault` comments)

class Controller {
public:
    Controller()
    :videoFX(0)
    {};
    
    void init();
    
    WebSystem::Connection connection;
    
    // Web system events
    void onHashTagCount(Json::Value body);
    void onShoutout(Json::Value body);
    void onCommand(Json::Value body);
    
    // Vide effects
    void setVideoFX( VideoFX *vfx ) {
        videoFX = vfx;
    }
    
    void decayVideoFXToDefault();
    
private:
    
    VideoFX *videoFX;
    
};