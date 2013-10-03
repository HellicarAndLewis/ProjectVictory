#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	gui.init();
	for(int i=0; i<32; i++) {
		gui.add(dummy, "Filler");
	}

	auto scroll = ofPtr<Panels::Scroll>(new Panels::Scroll());
	gui.add(scroll);

	dummy.allocate(512, 512, OF_IMAGE_GRAYSCALE);
	dummy.getPixelsRef().set(0, 0);
	dummy.update();

	for(int i=0; i<10; i++) {
		auto element = ElementPtr(new Element());
		element->setBounds(ofRectangle(0, 0, 200, 20));
		element->onDraw += [element, i] (DrawArguments&) {
			ofPushStyle();
			ofSetColor(100, 0, 0);
			ofFill();
			ofRect(0, 0, element->getWidth(), element->getHeight());
			ofPopStyle();
			ofDrawBitmapString(ofToString(i) + "......", 5, 15);
		};
		element->onMouseReleased += [i] (MouseArguments& args) {
			ofLogNotice() << i;
		};
		scroll->add(element);
	}

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
