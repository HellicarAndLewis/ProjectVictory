#pragma once

#include "ofxBlackmagic.h"
#include "ofxCvGui.h"
#include "ofMain.h"
#include "ofxVMIG.h"

using namespace ofxCvGui;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		Builder gui;
		ofPtr<ofxVMIG::Inspector> inspector;
		ofPtr<ofxVMIG::Settings> settings;
		ofPtr<ofxVMIG::Modules::VideoMatrix> videoMatrix;
		ofPtr<ofxVMIG::Modules::Crossfader> crossfader;
};
