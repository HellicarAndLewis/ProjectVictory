#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAntiAliasing();
    ofSetVerticalSync( true );
    ofSetFrameRate( 30 );
    
    #ifdef DISABLE_STREAMING
    cout << "Disabled streaming: compile with 'Release' to enable" << endl;
    #else
    cout << "Enabled streaming: compile with 'Release NoStream' to disable" << endl;
    #endif

    ofSetLogLevel( OF_LOG_WARNING );
    w = 1280;
    h = 720;
    int flowW = w/4;
    int flowH = h/4;
    
    videoFeedController.init();
    
    vfx1.init();
    vfx1.setVideoSource( videoFeedController.videoSource );
    
    vfx1.setColor( colors[colorIndex] );
    
    big = &vfx1;
    
    videoFXExporter.setVideoFX( &vfx1 );
    
    overlay.init();
    
    websystemController.setVideoFX( &vfx1 );
    websystemController.setOverlay( &overlay );
    websystemController.init();
    

#ifndef DISABLE_STREAMING
    // MULTI STREAMER
    // --------------------------------------------------
    int fps = 15;
    if(!streamer.setup("streamer.xml", ofGetWidth(), ofGetHeight(), fps)) {
      printf("error: cannot setup the streamer.\n");
      ::exit(EXIT_FAILURE);
    }

    if(!streamer.start()) {
      printf("error: cannot start the streamer.\n");
      ::exit(EXIT_FAILURE);
    }

    sound_stream.listDevices();
    sound_stream.setup(this, 0, 2, 44100, 1024, 4);
#endif
    
}

void testApp::audioIn(float* input, int nsize, int nchannels) {
    
#ifndef DISABLE_STREAMING
    size_t nsamples = nsize * nchannels;
    for(size_t i = 0; i < nsamples; ++i) {
        input[i] *= 32768.0f;
    }

    streamer.addAudio(input, nsize, nchannels);
#endif
}

//--------------------------------------------------------------
void testApp::update(){
    if (ofGetElapsedTimef() < 2.0f) { return; }
    
    websystemController.update();
    
    videoFeedController.update();
    
    vfx1.setVideoSource( videoFeedController.videoSource );
    vfx1.update( videoFeedController.videoSource->isFrameNew() );
    
    overlay.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if (ofGetElapsedTimef() < 2.0f) { return; }
    
#ifndef DISABLE_STREAMING
    if(streamer.wantsNewFrame()) {
        streamer.beginGrab();
            drawInternal();
        streamer.endGrab();
    }
#endif

    drawInternal();
        
}

void testApp::drawInternal() {
    
    big->draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    
    overlay.draw();
    
    // Screen shot saving
    string nextScreenshotName = websystemController.getNextScreenShotFilename();
    if ( nextScreenshotName != "" ) {
        ofSaveScreen( "screenshots/" + nextScreenshotName + ".png");
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    cout << key << endl;
    
    if ( key == 'a' ) {
        overlay.voteDisplay.addVote( overlay.voteDisplay.topic1 );
    }
    else if ( key == 'b' ) {
        overlay.voteDisplay.addVote( overlay.voteDisplay.topic2 );
    }
    
    if ( key == 'k' ) {
        vfx1.reloadShaders();
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
