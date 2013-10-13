#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAntiAliasing();
    ofSetVerticalSync( true );
    ofSetFrameRate( 60 );
    
    ofPixels pix;
    
    
    #ifdef DISABLE_STREAMING
    cout << "Disabled streaming: compile with 'Release' to enable" << endl;
    #else
    cout << "Enabled streaming: compile with 'Release NoStream' to disable" << endl;
    #endif

    ofSetLogLevel( OF_LOG_WARNING );
    
    //    videoFeedController.init();

    vfx1.init();
    // vfx1.setVideoSource( videoFeedController.videoSource );
    
    vfx1.setColor( colors[colorIndex] );

    big = &vfx1;
    
    videoFXExporter.setVideoFX( &vfx1 );
    
    overlay.init();
    
    websystemController.setVideoFX( &vfx1 );
    websystemController.setOverlay( &overlay );
    websystemController.init();
    
#if !defined(DISABLE_STREAMING)
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
  
    if(!ldeck.setup(0, bmdFormat8BitYUV, bmdModeHD1080p24)) {
      //if(!ldeck.setup(0, bmdFormat8BitYUV, bmdModePAL)) {
      printf("error: cannot get decklink setup.\n");
      ::exit(EXIT_FAILURE);
    }
  
    if(!ldeck.start()) {
      printf("error: cannot start the decklink device.\n");
      ::exit(EXIT_FAILURE);
    }

    int lw = ldeck.getWidth();
    int lh = ldeck.getHeight();
    if(lw < 0 || lh < 0) {
      printf("error: cannot get the correct width and height from the decklink device for the current format.\n");
      ::exit(EXIT_FAILURE);
    }

    ldeck_started = false;
    ldeck_new_img = false;
    ldeck_img.allocate(lw, lh, OF_IMAGE_COLOR);
    //unsigned char* p = new unsigned char[lw * lh * 3];
    //memset((char*)p, 0x00, (lw * lh * 3));
    //ldeck_img.setFromPixels(p, lw, lh, OF_IMAGE_COLOR);
    printf("%d x %d\n", lw, lh);

    sound_stream.listDevices();
    sound_stream.setup(this, 0, 2, 44100, 1024, 4);
#endif

    if(!grab_saver.setup(ofGetWidth(), ofGetHeight(), 20)) {
      printf("error: cannot start the screen grab saver.\n");
      ::exit(EXIT_FAILURE);
    }

#if !defined(DISABLE_STREAMING)
    if(!vfx1.setVideoSource(&ldeck_img)) {
      printf("error: cannot set video source.\n");
      ::exit(EXIT_FAILURE);
    }
#else 
    video_grabber.setDeviceID(0);
    video_grabber.setDesiredFrameRate(60);
    video_grabber.initGrabber(640,480);

    if(!vfx1.setVideoSource(&video_grabber)) {
      printf("error: cannot set the video source.\n");
      ::exit(EXIT_FAILURE);
    }
#endif

    
    // we don't want the gui to be drawn on the draw event
    /*
    vfx1.disableGuiEvents();
    websystemController.disableGuiEvents();
    overlay.disableGuiEvents();
    videoFXExporter.disableGuiEvents();
    videoFeedController.disableGuiEvents();
    */
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

#if defined(DISABLE_STREAMING)
    video_grabber.update();
    if(video_grabber.isFrameNew()) {
      vfx1.update(true);
    }
    else {
      vfx1.update(false);
    }
#else
    streamer.update();
    if(ldeck.hasNewFrame()) {
      vfx1.update(true);
    }
    else {
      vfx1.update(false);
    }
#endif
    
    websystemController.update();
    
#if defined(DISABLE_STREAMING) 
    /*
    if ( videoFeedController.update() ) {
        vfx1.setVideoSource( videoFeedController.videoSource );
    }
    */
#else
    if(ldeck_new_img) {
      if(!ldeck_started) {
        vfx1.setVideoSource(&ldeck_img);
        ldeck_started = true;
      }
      ldeck_new_img = false;
    }

#endif    
    
    
    overlay.update();


}

//--------------------------------------------------------------
void testApp::draw(){
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (ofGetElapsedTimef() < 2.0f) { return; }
    
#ifndef DISABLE_STREAMING
    // get decklink image.
    if(ldeck.hasNewFrame()) {

      ldeck.lock();
      {
        unsigned char* ptr = ldeck.getPixels();
        if(ptr) {
          ldeck_img.setFromPixels(ptr, ldeck.getWidth(), ldeck.getHeight(), OF_IMAGE_COLOR);
        }
        else {
          printf("error: we're getting an invalid pixel pointer from the ldeck addon.\n");
        }
      }
      ldeck.unlock();
      ldeck.resetHasNewFrame();
      ldeck_new_img = true;
    }
   
    // do we need to stream?
    if(streamer.wantsNewFrame()) {
      streamer.beginGrab();
        drawInternal();
      streamer.endGrab();
    }

#endif

    drawInternal();

    //ldeck_img.draw(0,0,ofGetWidth(), ofGetHeight()); // just testing the output of the decklink 
    /*
    videoFXExporter.drawGUI();
    overlay.drawGUI();
    websystemController.drawGUI();
    vfx1.drawGUI();
    videoFeedController.drawGUI();
    */
    cout << "framerate " << ofGetFrameRate() << endl;
}

void testApp::drawInternal() {
    
  //big->draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    big->draw(0, 0, ofGetWidth(), ofGetHeight());
    overlay.draw();
    
    // Screen shot saving
#if 0
    string nextScreenshotName = websystemController.getNextScreenShotFilename();
    if ( nextScreenshotName != "" ) {
        float now = ofGetElapsedTimef();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glReadBuffer(GL_BACK);
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        //Saving
        ofSaveViewport("screenshots/" + nextScreenshotName + ".png");

        //ofSaveScreen( "screenshots/" + nextScreenshotName + ".png");
        cout << "screen grab took " << ofToString(ofGetElapsedTimef()-now) << "seconds" << endl;
    }
#else

    screenshot_name = websystemController.getNextScreenShotFilename();
    if(screenshot_name.size()) {
      vfx1.hideGUI(); // <-- does 
      if(!grab_saver.grab(screenshot_name)) {
        printf("error: cannot grab - this may not happen! - allocate a bigger buffer in ScreenGrabSaver.\n");
        ::exit(EXIT_FAILURE);
      }
      vfx1.showGUI();
      screenshot_name.clear();
    }

#endif
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    // Toggle all UI's with space bar
    if ( key == ' ' ) {
        static bool showingGUI = true;
        showingGUI = !showingGUI;
        if (showingGUI) {
            vfx1.showGUI();
        } else {
            vfx1.hideGUI();
        }
        
        videoFXExporter.exporterGUI->setVisible( showingGUI );
        overlay.overlayGUI->setVisible( showingGUI );
        websystemController.websystemGUI->setVisible(showingGUI);
        websystemController.voteGUI->setVisible(showingGUI);
        //videoFeedController.gui->setVisible(showingGUI);
    }
    
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
      if ( --colorIndex < 0 ) {
            colorIndex = 6;
      }
      vfx1.setColor( colors[colorIndex] );
    }
    else if ( key == 61 ) {
      if ( ++colorIndex > 6 ) {
            colorIndex = 0;
      }
      vfx1.setColor( colors[colorIndex] );
    }
    else if(key == 'f') {
      ofToggleFullscreen();
      static bool is_fs = false;
      is_fs = !is_fs;
      //     ofSetFullScreen(is_fs);
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
