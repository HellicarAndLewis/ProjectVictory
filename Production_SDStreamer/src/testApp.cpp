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

    videoFeedController.init();
    overlay.init();
    

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

    //streamer.addAudio(input, nsize, nchannels);
#endif
}

//--------------------------------------------------------------
void testApp::update(){
    if (ofGetElapsedTimef() < 2.0f) { return; }
    
    videoFeedController.update();
    
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
    
    if ( videoFeedController.videoSource ) {
        videoFeedController.videoSource->draw(0, 0, 1280, 720);
    }
    
    overlay.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    

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
    videoFeedController.gui->saveSettings( "GUI/video.xml" );
    overlay.overlayGUI->saveSettings( "GUI/overlay.xml" );
}
