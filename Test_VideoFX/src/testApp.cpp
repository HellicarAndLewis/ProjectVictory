#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAntiAliasing();
    ofSetVerticalSync( true );
    ofSetFrameRate( 30 );
    

    ofSetLogLevel( OF_LOG_WARNING );
    w = 1280;
    h = 720;
    int flowW = w/4;
    int flowH = h/4;
    
//    videoGrabber.setDeviceID( 1 );
    videoGrabber.initGrabber( 640, 480 );
    videoSource = &videoGrabber;
    
    vfx1.init();
    vfx1.setVideoSource( &videoGrabber );
    
    vfx1.setColor( colors[colorIndex] );
    
    big = &vfx1;
    
    videoFXExporter.setVideoFX( &vfx1 );
    
    fbo.allocate( 1280,  720 );
    
    overlay.init();
}


//--------------------------------------------------------------
void testApp::update(){
    
    videoGrabber.update();
    
    vfx1.update( videoGrabber.isFrameNew() );
    
    overlay.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    fbo.begin();
    
    big->draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    
    overlay.draw();
    
    fbo.end();
    
    fbo.draw( 0, 0 );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    if ( key == 91 ) {
        overlay.voteDisplay.addVote( overlay.voteDisplay.topic1 );
    }
    else if ( key == 93 ) {
        overlay.voteDisplay.addVote( overlay.voteDisplay.topic2 );
    }
    
    if ( key == 'k' ) {
        vfx1.reloadShaders();
    }
    if ( key == '0' ) {
        ofPixels pixels;
        fbo.readToPixels( pixels );
        ofSaveImage( pixels, ofGetTimestampString() + ".png" );
    }
    else if ( key == 161 ) { // tilda key
        big->hideGUI();
        videoFXExporter.exporterGUI->setVisible( false );
        overlay.overlayGUI->setVisible( false );
    }
    else if ( key == '1' ) {
        big = &vfx1;
        big->showGUI();
        videoFXExporter.exporterGUI->setVisible( true );
        overlay.overlayGUI->setVisible( true );
    }
    else if ( key == 45 ) {
        if ( --colorIndex < 0 )
            colorIndex = 6;
        vfx1.setColor( colors[colorIndex] );
    }
    else if ( key == 61 ) {
        if ( ++colorIndex > 6 )
            colorIndex = 0;
        vfx1.setColor( colors[colorIndex] );
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
    
}

void testApp::exit() {
    vfx1.exit();
    videoFXExporter.exporterGUI->saveSettings("GUI/exporter.xml");
    overlay.overlayGUI->saveSettings( "GUI/overlay.xml" );
}
