#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "FarnebackPlus.h"
#include "CircularTexture.h"
#include "KhronosEffect.h"
#include "VideoFX.h"
#include "Controller.h"

class testApp : public ofBaseApp{

	public:
    
        int w, h;
    
    ofxFluid fluid;
    
        ofVideoGrabber videoGrabber;
        ofBaseVideoDraws *videoSource;
    
        VideoFX vfx;
    
		void setup();
        void setupGUI();
		void update();
		void draw();
        void guiEvent(ofxUIEventArgs &e);
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        Controller controller;
		
};
