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
#include <ofxVideoStreamer/ofxVideoStreamer.h>
#include <ofxVideoStreamer/ofxMultiVideoStreamer.h>


#define USE_VIDEOSTREAMER 1
#define USE_MULTI_VIDEOSTREAMER 0

class testApp : public ofBaseApp{

 public:
    
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
    
  ofVideoGrabber videoGrabber;
  ofVideoPlayer videoPlayer;
  ofBaseVideoDraws *videoSource;
    
  VideoFX vfx1, vfx2;
  VideoFX *big;
  VideoFX *small;
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
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  void audioIn(float* input, int nsize, int nchannels);
		

  VideoFXExporter videoFXExporter;

#if USE_VIDEOSTREAMER
  ofxVideoStreamer streamer;
#endif

#if USE_MULTI_VIDEOSTREAMER
  ofxMultiVideoStreamer streamer;
#endif

  ofSoundStream sound_stream;
};
