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
    lastSwitchTimeSecs(1.0f),
    videoSource(0)
    {};
    
    // Lifecycle
    void init();
    void update();
    
    // UI
    void guiEvent(ofxUIEventArgs &e);
    vector<VideoDeviceToggle *>videoToggles;
    ofxUISuperCanvas *gui;
    
    // Video
    ofBaseVideoDraws *videoSource;
    ofVideoDevices devices;
    
    
protected:
    float toggleEveryMins;
    float lastSwitchTimeSecs;
};