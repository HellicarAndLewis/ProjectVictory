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
    // Returns true if the video source did change
    bool update();
    
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