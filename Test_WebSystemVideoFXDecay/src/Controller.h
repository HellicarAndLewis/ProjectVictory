#pragma once
#include "ofMain.h"
#include "WebSystem.h"
#include "VideoFXExporter.h"
class VideoFX;

class Controller {
public:
    Controller()
    :videoFX(0)
    {};
    
    void init();
    
    WebSystem::Connection connection;
    VideoFXExporter videoFxExporter;
    
    // Web system events
    void onHashTagCount(Json::Value body);
    void onShoutout(Json::Value body);
    void onCommand(Json::Value body);
    
    // Vide effects
    void setVideoFX( VideoFX *vfx ) {
        videoFX = vfx;
        videoFxExporter.setVideoFX( vfx );
    }
    
    void decayVideoFXToDefault();
    
private:
    
    VideoFX *videoFX;
    
};