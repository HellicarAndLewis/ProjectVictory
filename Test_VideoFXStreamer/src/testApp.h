#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "FarnebackPlus.h"
#include "CircularTexture.h"
#include "KhronosEffect.h"
#include "VideoFX.h"
#include <ofxVideoStreamer/ofxVideoStreamerScreenCapture.h>

class testApp : public ofBaseApp{

	public:
    
        int w, h;
    
    ofxFluid fluid;
    
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
        ofBaseVideoDraws *videoSource;
    
        VideoFX vfx;
        ofxUICanvas *videoGUI;
    
		void setup();
        void setupGUI();
		void update();
		void draw();
        void guiEvent(ofxUIEventArgs &e);
        void exit();

		void keyPressed(int key);
    
    ofxVideoStreamerScreenCapture screen_capture;
		
};
