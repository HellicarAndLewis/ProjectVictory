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
    
    ofSetWindowShape( 1280, 720 );
    
    setupGUI();
    
//    videoGrabber.setDeviceID( 1 );
    videoGrabber.initGrabber( w, h );
    videoSource = &videoGrabber;
    
    videoPlayer.loadMovie( "going to the store.mp4" );
    videoPlayer.setLoopState( OF_LOOP_NORMAL );
    videoPlayer.play();
    if ( ((ofxUIToggle*)videoGUI->getWidget("MUTE"))->getValue() )
        videoPlayer.setVolume( 0.0 );
    
    vfx1.init();
    vfx1.setVideoSource( &videoGrabber );
    
    vfx2.init();
    vfx2.setOpticalFlowEnabled( false );
    vfx2.hideGUI();
    vfx2.setVideoSource( &videoPlayer );
    
    vfx1.setColor( colors[colorIndex] );
    vfx2.setColor( colors[colorIndex] );
    
    big = &vfx1;
    small = &vfx2;
    
    videoFXExporter.setVideoFX( &vfx1 );
}

void testApp::setupGUI() {
    
    videoGUI = new ofxUICanvas( ofGetWidth()-200, 0, 200, 200 );
    videoGUI->setColorBack( ofColor( ofColor::orange, 150 ) );
//    videoGUI->addSpacer();
//    videoGUI->addToggle( "USE WEBCAM", true );
    videoGUI->addSlider( "PLAYHEAD", 0.0, 1.0, 0.0 );
    videoGUI->addToggle( "MUTE", false );
    videoGUI->autoSizeToFitWidgets();
    ofAddListener( videoGUI->newGUIEvent, this, &testApp::guiEvent );
    videoGUI->loadSettings("GUI/video.xml");
    
    // playhead always at 0
    ((ofxUISlider*)videoGUI->getWidget( "PLAYHEAD" ))->setValue(0.0);
//    ((ofxUIToggle*)videoGUI->getWidget( "USE WEBCAM" ))->setValue( true );
}

void testApp::guiEvent(ofxUIEventArgs &e) {
    string name = e.widget->getName();
    
    if ( name == "PLAYHEAD" && videoPlayer.isPlaying() )
        videoPlayer.setPosition( e.getSlider()->getValue() );
    else if ( name == "MUTE" )
        videoPlayer.setVolume( e.getToggle()->getValue() ? 0.0 : 1.0 );
    else if ( name == "USE WEBCAM" ) {
        if ( e.getToggle()->getValue() ) {
            if ( videoSource != &videoGrabber ) {
                cout << "switching back to webcam" << endl;
                videoSource = &videoGrabber;
                vfx1.setVideoSource( videoSource );
                videoPlayer.stop();
            }
        }
        else {
            // if there is no video loaded, we stick with webcam
            if ( !videoPlayer.isLoaded() ) {
                cout << "no video loaded... sticking with webcam" << endl;
                e.getToggle()->setValue( true );
            }
            else {
                // make sure the video player is still playing
                if ( !videoPlayer.isPlaying() ) videoPlayer.play();
                videoSource = &videoPlayer;
                vfx1.setVideoSource( videoSource );
                videoPlayer.play();
            }
        }
        
    }
}

//--------------------------------------------------------------
void testApp::update(){
    if ( videoPlayer.isPlaying() ) {
        videoPlayer.update();
        ((ofxUISlider*)videoGUI->getWidget( "PLAYHEAD" ))->setValue( videoPlayer.getPosition() );
    }
    
    videoGrabber.update();
    
//    if ( videoGrabber.isFrameNew() )
        vfx1.update( videoGrabber.isFrameNew() );
//    if ( videoPlayer.isFrameNew() )
        vfx2.update( videoPlayer.isFrameNew() );
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    big->draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    small->draw( ofGetWidth()*.75, ofGetHeight()*.75, ofGetWidth()*.25, ofGetHeight()*.25 );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if ( key == 'k' ) {
        vfx1.reloadShaders();
        vfx2.reloadShaders();
    }
    else if ( key == 'h' ) {
        big->hideGUI();
        small->hideGUI();
    }
    else if ( key == '1' ) {
        big = &vfx1;
        small = &vfx2;
        big->showGUI();
        small->hideGUI();
    }
    else if ( key == '2' ) {
        big = &vfx2;
        small = &vfx1;
        big->showGUI();
        small->hideGUI();
    }
    else if ( key == 45 ) {
        if ( --colorIndex < 0 )
            colorIndex = 6;
        vfx1.setColor( colors[colorIndex] );
        vfx2.setColor( colors[colorIndex] );
    }
    else if ( key == 61 ) {
        if ( ++colorIndex > 6 )
            colorIndex = 0;
        vfx1.setColor( colors[colorIndex] );
        vfx2.setColor( colors[colorIndex] );
    }
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
    
    if ( dragInfo.files.size() == 1 ) {
        string filename = dragInfo.files[0];
        cout << "filename: " << filename << endl;
        string extension = filename.substr( filename.length()-3, 3 );
        if ( extension == "mov" || extension == "mp4" ) {
            cout << "its a movie" << endl;
            cout << ofToDataPath( dragInfo.files[0] ) << endl;
            videoPlayer.loadMovie( dragInfo.files[0] );
            videoPlayer.play();
//            videoSource = &videoPlayer;
//            vfx1.setVideoSource( videoSource );
//            ((ofxUIToggle*)videoGUI->getWidget("USE WEBCAM"))->setValue( false );
            
            if ( ((ofxUIToggle*)videoGUI->getWidget("MUTE"))->getValue() )
                videoPlayer.setVolume( 0.0 );
        }
    }
    
}

void testApp::exit() {
    vfx1.exit();
    vfx2.exit();
    videoGUI->saveSettings("GUI/video.xml");
}
