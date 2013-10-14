#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "FarnebackPlus.h"
#include "CircularTexture.h"
#include "KhronosEffect.h"
#include "VideoFX.h"
#include "VideoFXExporter.h"
#include "Overlay.h"
#include "WebSystemController.h"
#include "ScreenGrabSaver.h"
#include "StreamerGUI.h"

//#define DISABLE_STREAMING 1
#ifndef DISABLE_STREAMING
#include <ofxVideoStreamer/ofxMultiVideoStreamer.h>
#include <ofxLinkDeck/ofxLinkDeck.h>
#endif

//#include "VideoFeedController.h"

class testApp : public ofBaseApp{
public:

    int w, h;

    Overlay overlay;

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

    //ofVideoGrabber videoGrabber;
    ofBaseVideoDraws *videoSource;

    VideoFX vfx1;
    VideoFX *big;

    void setup();
    void update();
    void updateGUI();
    void draw();
    void drawInternal();    
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void audioIn(float* input, int nsize, int nchannels); /* the audio input for the video streamer */
        
    VideoFXExporter videoFXExporter;
    WebSystemController websystemController;
    //    VideoFeedController videoFeedController;
    ScreenGrabSaver grab_saver;
    std::string screenshot_name;
    
#ifndef DISABLE_STREAMING
    bool ldeck_started;
    bool ldeck_new_img;
    ofImage ldeck_img;
    ofxLinkDeck ldeck;
    ofxMultiVideoStreamer streamer; /* this is handling the video streaming */
    ofSoundStream sound_stream; /* we use a sound stream to get audio into the video stream */
#else 
    ofVideoGrabber video_grabber;
#endif
    
    ofImage farne_img1; // testing ..
    ofImage farne_img2; // testing ..
    ofImage* farne_img; 

    StreamerGUI gui;
};
