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

    vfx1.init();
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
    int fps = 25;
    if(!streamer.setup("streamer.xml", ofGetWidth(), ofGetHeight(), fps)) {
      printf("error: cannot setup the streamer.\n");
      ::exit(EXIT_FAILURE);
    }

    if(!streamer.start()) {
      printf("error: cannot start the streamer.\n");
      ::exit(EXIT_FAILURE);
    }
  
    //if(!ldeck.setup(0, bmdFormat8BitYUV, bmdModeHD1080p24)) {
    if(!ldeck.setup(0, bmdFormat8BitYUV, bmdModePAL)) {
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
    video_grabber.setDesiredFrameRate(25);
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

    // farne_img1.loadImage("farne1.jpg");
    //farne_img2.loadImage("farne2.jpg");
    // farne_img = &farne_img2;
    ///vfx1.setVideoSource(farne_img);

    gui.setup();
}

void testApp::audioIn(float* input, int nsize, int nchannels) {

  return;

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
  //    if (ofGetElapsedTimef() < 2.0f) { return; }
  updateGUI();
  
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
#endif
    
    websystemController.update();

    overlay.update();
  
}

//--------------------------------------------------------------
void testApp::draw(){
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
#ifndef DISABLE_STREAMING
    // get decklink image.
    if(ldeck.hasNewFrame()) {

      ldeck.lock();
      {
        unsigned char* ptr = ldeck.getPixels();
        if(ptr) {
          ldeck_img.setFromPixels(ptr, ldeck.getWidth(), ldeck.getHeight(), OF_IMAGE_COLOR);
          ldeck_img.update();
        }
        else {
          printf("error: we're getting an invalid pixel pointer from the ldeck addon.\n");
        }
      }
      ldeck.unlock();
      ldeck.resetHasNewFrame();

      vfx1.update(true);
      ldeck_new_img = true;
    }
    else {
      vfx1.update(false);
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

    //vfx1.fb.draw(0, 0, ofGetWidth(), ofGetHeight());
    //cout << "framerate " << ofGetFrameRate() << endl;
    gui.draw();
}

void testApp::drawInternal() {
    
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

  // when the key pressed is handled by the gui we do not handle it
  if(gui.onKeyPressed(key)) {
    return;
  }
  /*
  if(key == 't') {
    std::string input = ofSystemTextBoxDialog("what?");
    printf(">>. %s\n", input.c_str());
  }
  */
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
      /*
        big = &vfx1;
        big->showGUI();
        videoFXExporter.exporterGUI->setVisible( true );
        overlay.overlayGUI->setVisible( true );
      */
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
//    else if(key == 'f') {
//      ofToggleFullscreen();
//      static bool is_fs = false;
//      is_fs = !is_fs  ;
//      //     ofSetFullScreen(is_fs);
//    }
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

void testApp::updateGUI() {
  
  /* receive OSC */
  gui.update();

  /* update websystem */
  websystemController.setEnabled(gui.web_is_enabled);
  websystemController.setShoutOutsEnabled(gui.web_shoutouts_enabled);
  websystemController.setCommandsEnabled(gui.web_commands_enabled);
  websystemController.setShouldDecayEffects(gui.web_decay_effects);
  websystemController.setCountHashTags(gui.web_count_hash_tags);

  /* update effects */
  vfx1.setKhronosEnabled(gui.fx_khronos_enabled);
  vfx1.setBadTVEnabled(gui.fx_bad_tv_enabled);
  vfx1.setScanLinesEnabled(gui.fx_scanlines_enabled);
  vfx1.setRGBShiftEnabled(gui.fx_rgb_shift_enabled);
  vfx1.setColorMapEnabled(gui.fx_color_map_enabled);
  vfx1.setFlowLinesEnabled(gui.fx_flow_lines_enabled);
  vfx1.setGridDistortEnabled(gui.fx_grid_distort_enabled);

  /* khronos */
  if(gui.fx_khronos_enabled) {
    vfx1.khronosEffect.setAmount(gui.khronos_amount);
    vfx1.khronosEffect.setMagScaler(gui.khronos_mag_scaler);
  }
  
  /* color map */
  if(gui.fx_color_map_enabled) {
    vfx1.colorMapEffect.setAmount(gui.colmap_amount);
  }

  /* scan lines */
  if(gui.fx_scanlines_enabled) {
    vfx1.scanlinesEffect.setAmount(gui.sl_amount);
    vfx1.scanlinesEffect.setCount(gui.sl_count);
    vfx1.scanlinesEffect.setSIntensity(gui.sl_s_intensity);
    vfx1.scanlinesEffect.setNIntensity(gui.sl_n_intensity);
  }

  /* flow lines */
  if(gui.fx_flow_lines_enabled) {
    vfx1.flowLinesEffect.setAmount(gui.flow_amount);
    vfx1.flowLinesEffect.setStepSize(gui.flow_step_size);
    vfx1.flowLinesEffect.setLineScale(gui.flow_line_scale);
    vfx1.flowLinesEffect.setThreshold(gui.flow_threshold);
  }

  /* rgb shift */
  if(gui.fx_rgb_shift_enabled) {
    vfx1.rgbShiftEffect.setAmount(gui.rgb_amount);
    vfx1.rgbShiftEffect.setRGBAmount(gui.rgb_rgb_amount);
    vfx1.rgbShiftEffect.setAngle(gui.rgb_angle);
  }

  /* bad tv efect */
  if(gui.fx_bad_tv_enabled) {
    vfx1.badTVEffect.setAmount(gui.btv_amount);
    vfx1.badTVEffect.setThickDistort(gui.btv_thick_distort);
    vfx1.badTVEffect.setFineDistort(gui.btv_fine_distort);
    vfx1.badTVEffect.setDistortSpeed(gui.btv_distort_speed);
    vfx1.badTVEffect.setRollSpeed(gui.btv_roll_speed);
  }

  if(gui.fx_grid_distort_enabled) {
    vfx1.gridDistortEffect.setAmount(gui.grid_amount);
    vfx1.gridDistortEffect.setStepSize(gui.grid_step_size);
    vfx1.gridDistortEffect.setParticleRadius(gui.grid_particle_radius);
    vfx1.gridDistortEffect.setForceScale(gui.grid_force_scale);
    vfx1.gridDistortEffect.setSpringStrength(gui.grid_spring_strength);
    vfx1.gridDistortEffect.setDrag(gui.grid_drag);
    vfx1.gridDistortEffect.setShowGrid(gui.grid_show_grid);
  }
}
