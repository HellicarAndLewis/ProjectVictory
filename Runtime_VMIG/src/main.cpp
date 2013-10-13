#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1920,1080*3,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREENs
	// pass in width and height too:
	ofRunApp(new ofApp());

}
