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

class testApp : public ofBaseApp{

	public:
    
        int w, h;
    
        ofFbo fbo;
    
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
    
        ofVideoGrabber videoGrabber;
        ofBaseVideoDraws *videoSource;
    
        VideoFX vfx1, vfx2;
        VideoFX *big;
    
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        VideoFXExporter videoFXExporter;    
};
