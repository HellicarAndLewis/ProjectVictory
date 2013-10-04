#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

  int w = ofGetWidth();
  int h = ofGetHeight();


 
  if(!screen_capture.setup("streamer.xml", w, h, w, h)) {
    printf("error: cannot load the streamer settings.\n");
    ::exit(EXIT_FAILURE);
  }

  if(!screen_capture.start()) {
    printf("error: cannot start the screen capture.\n");
    ::exit(EXIT_FAILURE);
  }

}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){

  int hw = ofGetWidth() * 0.5;
  int hh = ofGetHeight() * 0.5;

  float t = ofGetElapsedTimeMillis();
  float s = 0.5 + sin(t * 0.0001) * 0.5;
  ofSetColor(s * 255, 0.0, 255);

  if(screen_capture.hasNewFrame()) {
    screen_capture.beginGrab();
       ofCircle(hw, hh, 100 + sin(ofGetElapsedTimeMillis() * 0.001) * 100);
    screen_capture.endGrab();
	//screen_capture.draw();
  }
//	else {
	glDisable(GL_DEPTH_TEST);
	ofCircle(hw, hh, 100 + sin(ofGetElapsedTimeMillis() * 0.001) * 100);
//	}
        
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
