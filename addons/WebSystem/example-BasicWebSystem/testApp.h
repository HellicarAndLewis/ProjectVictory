#pragma once

#include "ofMain.h"
#include "WebSystem.h"
#include "ShoutoutRenderer.h"
#include "HashtagRenderer.h"
#include "VideoFX.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    // Web system
    WebSystem::Connection connection;
    
    // VideoFX
    VideoFX vfx;
    ofVideoGrabber webcam;
    
    // Renderers (to be replaces by modules)
    ShoutoutRenderer shoutoutRenderer;
    HashtagRenderer hashtagRenderer;
    
    // Web system events
    void onHashTagCount(Json::Value body);
    void onShoutout(Json::Value body);
    void onCommand(Json::Value body); 
    
};
