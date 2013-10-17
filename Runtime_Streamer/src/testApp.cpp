#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  ofEnableAntiAliasing();
  ofSetVerticalSync( true );
  ofSetFrameRate(60);
  ofSetWindowTitle("Victory Visualizer");

  is_fullscreen = false;

#if !USE_STREAMING
  cout << "Disabled streaming: compile with 'Release' to enable" << endl;
#else
  cout << "Enabled streaming: compile with 'Release NoStream' to disable" << endl;
#endif

  ofSetLogLevel( OF_LOG_WARNING );

  vfx.init();
  vfx.setColor( colors[colorIndex] );

  overlay.init();
    
  websystemController.setVideoFX( &vfx );
  websystemController.setOverlay( &overlay );
  websystemController.init();

  websystemController.setGui( &gui );
    
#if USE_STREAMING

  int video_w = 1280;
  int video_h = 720;
  int fps = 25;
  if(!streamer.setup("streamer.xml", video_w, video_h, fps)) {
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

  if(!grab_saver.setup(ofGetWidth(), ofGetHeight(), 200)) { // last params is number of preallocated frames
    printf("error: cannot start the screen grab saver.\n");
    ::exit(EXIT_FAILURE);
  }

#if USE_DECKLINK
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

  if(!vfx.setVideoSource(&ldeck_img)) {
    printf("error: cannot set video source.\n");
    ::exit(EXIT_FAILURE);
  }
#endif

#if USE_CAM
  video_grabber.setDeviceID(0);
  video_grabber.setDesiredFrameRate(25);
  video_grabber.initGrabber(640,480);

  if(!vfx.setVideoSource(&video_grabber)) {
    printf("error: cannot set the video source.\n");
    ::exit(EXIT_FAILURE);
  }
#endif

  gui.setup("remoteing.xml", false);

  // load in all the command overlay images
  for (size_t i=0; i<gui.command_overlay_images.size(); ++i) {
    CommandOverlayImage *overlayImage = gui.command_overlay_images[i];
    overlayImage->image.loadImage( overlayImage->file );
  }
}  


void testApp::audioIn(float* input, int nsize, int nchannels) {

#if USE_STREAMING && USE_AUDIO
  size_t nsamples = nsize * nchannels;
  for(size_t i = 0; i < nsamples; ++i) {
    input[i] *= 32768.0f;
  }
  streamer.addAudio(input, nsize, nchannels);
#endif

}

//--------------------------------------------------------------
void testApp::update(){

  updateGUI();

#if USE_STREAMING
  streamer.update();
#endif

#if USE_CAM
  video_grabber.update();
  if(video_grabber.isFrameNew()) {
    vfx.update(true);
  }
  else {
    vfx.update(false);
  } 

#endif

  websystemController.update();

  overlay.update();
}

//--------------------------------------------------------------
void testApp::draw(){

  screenshot_name = websystemController.getNextScreenShotFilename();
  
#if USE_DECKLINK

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

    vfx.update(true);
    ldeck_new_img = true;
  }
  else {
    vfx.update(false);
  }

#endif

#if USE_STREAMING
  // do we need to stream?
  if(streamer.wantsNewFrame()) {
    streamer.beginGrab();
    drawInternal();
    streamer.endGrab();
  }
#endif

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, ofGetWidth(), ofGetHeight());
  drawInternal();

  takeScreenGrab();
}

void testApp::drawInternal() {
  vfx.draw(0, 0, ofGetWidth(), ofGetHeight());
  // draw each command overlay image
  for ( std::vector<CommandOverlayImage*>::iterator it = gui.command_overlay_images.begin(); it != gui.command_overlay_images.end(); ++it ) {
      ofPushStyle();
      {
          ofSetColor(ofColor::white, (*it)->opacity * 255);
          (*it)->image.draw(0, 0);
          (*it)->decay(gui.web_decay_speed);
      }
      ofPopStyle();
  }
  // draw the other overlays, scroller, votes, text
  overlay.draw();
}

void testApp::takeScreenGrab() {
  if(screenshot_name.size()) {
    if(!grab_saver.grab(screenshot_name)) {
      printf("error: cannot grab - this may not happen! - allocate a bigger buffer in ScreenGrabSaver.\n");
      ::exit(EXIT_FAILURE);
    }
    screenshot_name.clear();
  }
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {

  // when the key pressed is handled by the gui we do not handle it
  if(gui.onKeyPressed(key)) {
    return;
  }

  // Toggle all UI's with space bar
  if ( key == 'a' ) {
    overlay.voteDisplay.addVote( overlay.voteDisplay.topic1 );
  }
  else if ( key == 'b' ) {
    overlay.voteDisplay.addVote( overlay.voteDisplay.topic2 );
  }
  else if ( key == 'k' ) {
    vfx.reloadShaders(); // @todo is this used? 
  }
  else if ( key == 45 ) {
    printf("color..\n");
    if ( --colorIndex < 0 ) {
      colorIndex = 6;
    }
    vfx.setColor( colors[colorIndex] );
  }
  else if ( key == 61 ) {
    if ( ++colorIndex > 6 ) {
      colorIndex = 0;
    }
    vfx.setColor( colors[colorIndex] );
  }
  else if(key == 'f') {
    is_fullscreen = !is_fullscreen;
    ofSetFullscreen(is_fullscreen);

    if(is_fullscreen) {
      ofHideCursor();
#ifdef __APPLE__
    CGDisplayHideCursor(NULL); 
#endif
    }
    else {
      ofShowCursor();
#ifdef __APPLE__
    CGDisplayShowCursor(NULL); 
#endif
    }
  }
  else if (key == 'g') {
    websystemController.triggerFakeScreenGrab();
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
  vfx.exit();
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
  websystemController.setDecayRate(gui.web_decay_speed);
  websystemController.setEffectImpact(gui.web_command_impact);

  /* update effects */
  vfx.setKhronosEnabled(gui.fx_khronos_enabled);
  vfx.setBadTVEnabled(gui.fx_bad_tv_enabled);
  vfx.setScanLinesEnabled(gui.fx_scanlines_enabled);
  vfx.setRGBShiftEnabled(gui.fx_rgb_shift_enabled);
  vfx.setColorMapEnabled(gui.fx_color_map_enabled);
  vfx.setFlowLinesEnabled(gui.fx_flow_lines_enabled);
  vfx.setGridDistortEnabled(gui.fx_grid_distort_enabled);

  /* khronos */
  if(gui.fx_khronos_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.khronosEffect.setAmount(gui.khronos_amount); 
    } else { 
      gui.khronos_amount = vfx.khronosEffect.amount;  
    }
    vfx.khronosEffect.setMagScaler(gui.khronos_mag_scaler);
  }
  
  /* color map */
  if(gui.fx_color_map_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.colorMapEffect.setAmount(gui.colmap_amount); 
    } else { 
      gui.colmap_amount = vfx.colorMapEffect.amount;  
    }
  }

  /* scan lines */
  if(gui.fx_scanlines_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.scanlinesEffect.setAmount(gui.sl_amount); 
    } else { 
      gui.sl_amount = vfx.scanlinesEffect.amount;  
    }
    vfx.scanlinesEffect.setCount(gui.sl_count);
    vfx.scanlinesEffect.setSIntensity(gui.sl_s_intensity);
    vfx.scanlinesEffect.setNIntensity(gui.sl_n_intensity);
  }

  /* flow lines */
  if(gui.fx_flow_lines_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.flowLinesEffect.setAmount(gui.flow_amount); 
    } else { 
      gui.flow_amount = vfx.flowLinesEffect.amount;  
    }
    vfx.flowLinesEffect.setStepSize(gui.flow_step_size);
    vfx.flowLinesEffect.setLineScale(gui.flow_line_scale);
    vfx.flowLinesEffect.setThreshold(gui.flow_threshold);
  }

  /* rgb shift */
  if(gui.fx_rgb_shift_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.rgbShiftEffect.setAmount(gui.rgb_amount); 
    } else { 
      gui.rgb_amount = vfx.rgbShiftEffect.amount;  
    }
    vfx.rgbShiftEffect.setRGBAmount(gui.rgb_rgb_amount);
    vfx.rgbShiftEffect.setAngle(gui.rgb_angle);
  }

  /* bad tv efect */
  if(gui.fx_bad_tv_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.badTVEffect.setAmount(gui.btv_amount); 
    } else { 
      gui.btv_amount = vfx.badTVEffect.amount;  
    }
    vfx.badTVEffect.setThickDistort(gui.btv_thick_distort);
    vfx.badTVEffect.setFineDistort(gui.btv_fine_distort);
    vfx.badTVEffect.setDistortSpeed(gui.btv_distort_speed);
    vfx.badTVEffect.setRollSpeed(gui.btv_roll_speed);
  }

  /* grid effect */
  if(gui.fx_grid_distort_enabled) {
    if (!gui.web_commands_enabled) { 
      vfx.gridDistortEffect.setAmount(gui.grid_amount); 
    } else { 
      gui.grid_amount = vfx.gridDistortEffect.amount;  
    }
    vfx.gridDistortEffect.setStepSize(gui.grid_step_size);
    vfx.gridDistortEffect.setParticleRadius(gui.grid_particle_radius);
    vfx.gridDistortEffect.setForceScale(gui.grid_force_scale);
    vfx.gridDistortEffect.setSpringStrength(gui.grid_spring_strength);
    vfx.gridDistortEffect.setDrag(gui.grid_drag);
    vfx.gridDistortEffect.setShowGrid(gui.grid_show_grid);
  }

  /* overlay text */
  if(gui.overlay_text.size()) {
    overlay.textOverlay.text = gui.overlay_text;
    gui.overlay_text.clear();
  }

  /* overlay images */
  overlay.setDrawOverlayImage(gui.overlay_image_enabled);
  overlay.setOverlayImageOpacity(gui.overlay_image_opacity);

  if(gui.didOverlayImageChange()) {

    if(gui.overlay_dx >= gui.overlay_images.size()) {
      printf("error: overlay_dx is bigger then the number of overlay images. something went wrong.. stopping now.\n");
      ::exit(EXIT_FAILURE);
    }
    
    overlay.loadAndShowImage(gui.overlay_images[gui.overlay_dx]->file);
    gui.resetOverlayImageChanged();
  }

  /* crawl */
  overlay.setCrawlSpeed(gui.crawl_speed);
  overlay.setCrawEnabled(gui.crawl_enabled);

  /* text overlay */
  overlay.setTextEnabled(gui.text_enabled);
  overlay.setTextSpacing(gui.text_spacing);
  overlay.setTextScale(gui.text_scale);
  overlay.setTextOpacity(gui.text_opacity);
}
