#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	this->videoMatrix = ofPtr<ofxVMIG::Modules::VideoMatrix>(new ofxVMIG::Modules::VideoMatrix());
	this->crossfader = ofPtr<ofxVMIG::Modules::Crossfader>(new ofxVMIG::Modules::Crossfader(1920, 1080));
	this->inspector = ofPtr<ofxVMIG::Inspector>(new ofxVMIG::Inspector());
	this->settings = ofPtr<ofxVMIG::Settings>(new ofxVMIG::Settings());

	gui.init();

	auto screen1Grid = Builder::makeGrid();
	auto screen2Grid = Builder::makeGrid();
	screen1Grid->setColsCount(1);
	//screen2Grid->setColsCount(2);


	//--
	//screen 1
	//
	auto AB0Grid = Builder::makeGrid();
	AB0Grid->setColsCount(3);
	AB0Grid->add(Builder::makePanel(this->videoMatrix->getInput(0).getTextureReference(), "A"));
	AB0Grid->add(Builder::makePanel(this->crossfader->getTarget(), "Result"));
	AB0Grid->add(Builder::makePanel(this->videoMatrix->getInput(1).getTextureReference(), "B"));
	
	{
		vector<float> heights;
		heights.push_back(1080 - 360);
		heights.push_back(360.0f);
		screen1Grid->setHeights(heights);
	}

	screen1Grid->add(this->videoMatrix);
	screen1Grid->add(AB0Grid);
	//
	//--


	//--
	//screen 2
	//	
	this->crossfader->setA(this->videoMatrix->getInput(0).getTextureReference());
	this->crossfader->setB(this->videoMatrix->getInput(1).getTextureReference());

	auto controlsGrid = Builder::makeGrid();
	controlsGrid->add(this->crossfader);

	{
		vector<float> widths;
		widths.push_back(1920 - 300);
		widths.push_back(300);
		screen2Grid->setWidths(widths);
	}

	screen2Grid->add(controlsGrid);
	screen2Grid->add(this->inspector);
	//
	//--


	auto rootGrid = Builder::makeGrid();
	rootGrid->setColsCount(1);
	gui.add(rootGrid);
	rootGrid->add(screen1Grid);
	rootGrid->add(screen2Grid);
	{
		vector<float> heights;
		heights.push_back(1080);
		heights.push_back(1080);
		rootGrid->setHeights(heights);
	}

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
