#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetWindowPosition(1080, 0);
	//ofSetWindowShape(1920, 1080 * 2);

	this->hubControl.setup();
	this->hubControl.setAddress("192.168.1.96");

	ofSetVerticalSync(true);
	this->input = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	this->hubControl.update();

	//do regularly
	if (ofGetFrameNum() % (60*2) == 0) { // it takes approximately 2 seconds to get a signal onto a monitor from switching
		input++;
		input %= 4;
		this->hubControl.setRoute(6, input + 9);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(ofToString(input), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
