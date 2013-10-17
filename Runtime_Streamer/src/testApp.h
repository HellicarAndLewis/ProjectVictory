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

#define USE_DECKLINK 0
#define USE_CAM 1
#define USE_STREAMING 0
#define USE_AUDIO 1
#define OUTPUT_WIDTH 1280
#define OUTPUT_HEIGHT 720

#if USE_CAM && USE_DECKLINK
#  error "No no... don't use both a webcam and decklink. Choose one."
#endif

#if USE_STREAMING
#  include <ofxVideoStreamer/ofxMultiVideoStreamer.h>
#  include <ofxLinkDeck/ofxLinkDeck.h>
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
    bool is_fullscreen;

#if USE_DECKLINK
    ofxLinkDeck ldeck;    
#endif

#if USE_CAM
    ofVideoGrabber video_grabber;
#endif

#if USE_STREAMING
    bool ldeck_started;
    bool ldeck_new_img;
    ofImage ldeck_img;
    ofxMultiVideoStreamer streamer; /* this is handling the video streaming */
    ofSoundStream sound_stream; /* we use a sound stream to get audio into the video stream */
#endif

};
