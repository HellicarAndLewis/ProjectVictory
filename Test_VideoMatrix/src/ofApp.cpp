#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	this->settings = ofPtr<ofxVMIG::Settings>(new ofxVMIG::Settings());
	this->videoMatrix = ofPtr<ofxVMIG::Modules::VideoMatrix>(new ofxVMIG::Modules::VideoMatrix());
	this->inspector = ofPtr<ofxVMIG::Inspector>(new ofxVMIG::Inspector());

	gui.init();

	this->gui.add(this->settings);
	this->gui.add(this->inspector);
	this->gui.add(this->videoMatrix);

	//ofSetWindowPosition(1080, 0);
	//ofSetWindowShape(1920, 1080 * 2);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
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
