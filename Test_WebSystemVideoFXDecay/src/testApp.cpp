#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAntiAliasing();

    ofSetLogLevel( OF_LOG_WARNING );
    w = 1280;
    h = 720;
    int flowW = w/4;
    int flowH = h/4;
    
    ofSetWindowShape( 1280, 720 );
    
    setupGUI();
    
    videoGrabber.initGrabber( w, h );
    videoSource = &videoGrabber;
    
    vfx.init();
    vfx.setVideoSource( &videoGrabber );
    
    // Add the video effects to the controller
    controller.init();
    controller.setVideoFX( &vfx );
}

void testApp::setupGUI() {
    
}

void testApp::guiEvent(ofxUIEventArgs &e) {
    
}

//--------------------------------------------------------------
void testApp::update(){
        
    videoGrabber.update();
    vfx.update( videoGrabber.isFrameNew() );
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // This should bring them down
    if (videoGrabber.isFrameNew()) {
        controller.decayVideoFXToDefault();
    }
    vfx.draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    
    ofDrawBitmapString( ofToString(ofGetFrameRate(), 3), ofPoint(20,20) );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

void testApp::exit() {
    vfx.exit();
}
