#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "SDOverlay.h"

#ifndef DISABLE_STREAMING
#include <ofxVideoStreamer/ofxMultiVideoStreamer.h>
#endif

#include "VideoFeedController.h"

class testApp : public ofBaseApp{
public:

    int w, h;

    SDOverlay overlay;
    
    void setup();
    void update();
    void draw();
    void drawInternal();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void audioIn(float* input, int nsize, int nchannels); /* the audio input for the video streamer */
    
    VideoFeedController videoFeedController;
    
#ifndef DISABLE_STREAMING
  ofxMultiVideoStreamer streamer; /* this is handling the video streaming */
  ofSoundStream sound_stream; /* we use a sound stream to get audio into the video stream */
#endif
    
};
