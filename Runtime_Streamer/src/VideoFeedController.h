#pragma once
#include "ofMain.h"
#include "ofxUI.h"
class VideoDeviceToggle;

typedef vector<ofVideoDevice> ofVideoDevices;
typedef vector<ofVideoDevice>::iterator ofVideoDevicesIt;

class VideoFeedController {
public:
    
    VideoFeedController()
    :toggleEveryMins(0.1f),
    lastSwitchTimeSecs(1.0f)
    {};
    
    // Lifecycle
    void init();
    void update();
    
    // UI
    void guiEvent(ofxUIEventArgs &e);
    vector<VideoDeviceToggle *>videoToggles;
    
    // Video
    ofBaseVideoDraws *videoSource;
    ofVideoDevices devices;
    
private:
    float toggleEveryMins;
    float lastSwitchTimeSecs;
    ofxUISuperCanvas *gui;
};