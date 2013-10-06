#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "FarnebackPlus.h"
#include "CircularTexture.h"
#include "KhronosEffect.h"
#include "VideoFX.h"
#include "VideoFXExporter.h"
#include "Crawl.h"
#include "VoteDisplay.h"
#include <ofxVideoStreamer/ofxVideoStreamerScreenCapture.h>

class testApp : public ofBaseApp{

public:
    //videoFX by aaron
    int w, h;

    ofxUISuperCanvas * overlayGUI;
    Crawl crawl;
    VoteDisplay voteDisplay;
    bool drawOverlayImage = true;
    float overlayImageOpacity = 1.0;
    ofImage overlayImage;

    ofColor colors[7] = {
        ofColor::red,
        ofColor::orange,
        ofColor::yellow,
        ofColor::green,
        ofColor::blue,
        ofColor::indigo,
        ofColor::violet
    };
    int colorIndex = 0;
    
    VideoFX vfx1;
    ofxUICanvas *videoGUI;

    void setup();
    void setupGUI();
    void update();
    void draw();
    void drawVideo();
    void overlayGuiEvent( ofxUIEventArgs &e );
    void guiEvent(ofxUIEventArgs &e);
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
        
    VideoFXExporter videoFXExporter;

    //streaming by roxlu
    ofxVideoStreamerScreenCapture streamer;
    
    //multiple cameras by jgl
    ofVideoGrabber videoGrabber;
    ofPtr<ofQTKitGrabber>	cameras;
    vector<string> videoDevices;
    void switchToOtherCamera();
};
