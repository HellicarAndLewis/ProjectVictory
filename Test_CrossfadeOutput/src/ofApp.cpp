#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	this->videoMatrix = ofPtr<ofxVMIG::Modules::VideoMatrix>(new ofxVMIG::Modules::VideoMatrix());
	this->crossfader = ofPtr<ofxVMIG::Modules::Crossfader>(new ofxVMIG::Modules::Crossfader(1920, 1080));

	gui.init();

	auto rootGrid = Builder::makeGrid();
	rootGrid->setColsCount(1);
	gui.add(rootGrid);

	auto resultPanel = Builder::makePanel(this->crossfader->getTarget().getTextureReference(), "Crossfader result");
	rootGrid->add(resultPanel);
	rootGrid->add(crossfader);

	this->crossfader->setA(this->videoMatrix->getInput(0).getTextureReference());
	this->crossfader->setB(this->videoMatrix->getInput(1).getTextureReference());

	ofSetWindowPosition(1080, 0);
	ofSetWindowShape(1920, 1080 * 2);
}

//--------------------------------------------------------------
void ofApp::update(){
	//we have to update manually for this example because we're not displaying the videoMatrix
	this->videoMatrix->update();
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
