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

#define DISABLE_STREAMING 1
#ifndef DISABLE_STREAMING
#include <ofxVideoStreamer/ofxMultiVideoStreamer.h>
#include <ofxLinkDeck/ofxLinkDeck.h>
#endif

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void updateGUI();
    void draw();
    void drawInternal();    
    void takeScreenGrab();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void audioIn(float* input, int nsize, int nchannels); /* the audio input for the video streamer */

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

    ofBaseVideoDraws *videoSource;
    VideoFX vfx;
    WebSystemController websystemController;
    ScreenGrabSaver grab_saver;
    std::string screenshot_name;
    StreamerGUI gui;
    
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


};
