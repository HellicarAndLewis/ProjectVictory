#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // >:(
//    ofEnableNormalizedTexCoords();
//    ofDisableArbTex();
//    ofEnableSmoothing();
    ofEnableAntiAliasing();

    ofSetLogLevel( OF_LOG_WARNING );
    w = 1280;
    h = 720;
    int flowW = w/4;
    int flowH = h/4;
    
    ofSetWindowShape( w, h );
    
    setupGUI();
    
//    videoGrabber.setDeviceID( 1 );
    videoGrabber.initGrabber( w, h );
    videoSource = &videoGrabber;
    
    vfx.init();
    vfx.setVideoSource( &videoGrabber );
    vfx.setColor( colors[colorIndex] );
    
    
    //
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    if(!screen_capture.setup("streamer.xml", w, h, 640, 360)) {
        printf("error: cannot load the streamer settings.\n");
        ::exit(EXIT_FAILURE);
    }
    
    if(!screen_capture.start()) {
        printf("error: cannot start the screen capture.\n");
        ::exit(EXIT_FAILURE);
    }
    //
}

void testApp::setupGUI() {
    
//    videoGUI = new ofxUICanvas( ofGetWidth()-200, 0, 200, 200 );
//    videoGUI->setColorBack( ofColor( ofColor::orange, 150 ) );
//    videoGUI->addSlider( "PLAYHEAD", 0.0, 1.0, 0.0 );
//    videoGUI->addToggle( "MUTE", false );
//    videoGUI->autoSizeToFitWidgets();
//    ofAddListener( videoGUI->newGUIEvent, this, &testApp::guiEvent );
//    videoGUI->loadSettings("GUI/video.xml");
//    
//    // playhead always at 0
//    ((ofxUISlider*)videoGUI->getWidget( "PLAYHEAD" ))->setValue(0.0);
////    ((ofxUIToggle*)videoGUI->getWidget( "USE WEBCAM" ))->setValue( true );
}

void testApp::guiEvent(ofxUIEventArgs &e) {
    string name = e.widget->getName();
}

//--------------------------------------------------------------
void testApp::update(){
    
    videoGrabber.update();
    vfx.update( videoGrabber.isFrameNew() );
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    static float lastScreenCaptureRequest = 0;
    
    if(screen_capture.hasNewFrame()) {
        screen_capture.beginGrab();
        // Draw the video effects
        vfx.draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
        screen_capture.endGrab();
        //screen_capture.draw();
        lastScreenCaptureRequest = ofGetElapsedTimef();
    }
    
    vfx.draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofRect(0,0,ofGetWidth(),20.0f);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofDrawBitmapString("Test_VideoFXStreamer", ofPoint(180.0f, 15.0f));
    
    if ( ofGetElapsedTimef() - lastScreenCaptureRequest < 2.0f) {
        ofDrawBitmapString("Screen is being captured for stream http://test.home.roxlu.com/rtmpd/", ofPoint(400.0f, 15.0f));
    } else {
        ofDrawBitmapString("Screen not being captured for stream", ofPoint(500.0f, 15.0f));
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}


void testApp::exit() {
    // vfx.exit();
    // videoGUI->saveSettings("GUI/video.xml");
}
