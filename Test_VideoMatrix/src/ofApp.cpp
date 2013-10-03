#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	this->settings = ofPtr<ofxVMIG::Settings>(new ofxVMIG::Settings());
	this->videoMatrix = ofPtr<ofxVMIG::Modules::VideoMatrix>(new ofxVMIG::Modules::VideoMatrix());
	this->inspector = ofPtr<ofxVMIG::Inspector>(new ofxVMIG::Inspector());

	gui.init();

	auto rootGrid = Builder::makeGrid();
	rootGrid->setColsCount(1);
	gui.add(rootGrid);

	auto ABCDGrid = Builder::makeGrid();
	ABCDGrid->add(ofxCvGui::Builder::makePanel(this->videoMatrix->getInput(0), "A"));
	ABCDGrid->add(ofxCvGui::Builder::makePanel(this->videoMatrix->getInput(1), "B"));
	ABCDGrid->add(ofxCvGui::Builder::makePanel(this->videoMatrix->getInput(2), "C"));
	ABCDGrid->add(ofxCvGui::Builder::makePanel(this->videoMatrix->getInput(3), "D"));

	auto settingsGrid = Builder::makeGrid();
	settingsGrid->add(this->settings);
	settingsGrid->add(this->inspector);

	rootGrid->add(this->videoMatrix);
	rootGrid->add(ABCDGrid);
	rootGrid->add(settingsGrid);

	ofSetWindowPosition(1080, 0);
	ofSetWindowShape(1920, 1080 * 2);
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
