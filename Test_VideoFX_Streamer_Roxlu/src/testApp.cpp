#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAntiAliasing();
    ofSetVerticalSync( true );
    ofSetFrameRate( 60.0 );
    

    ofSetLogLevel( OF_LOG_WARNING );
    w = 1280;
    h = 720;
    int flowW = w/4;
    int flowH = h/4;
    
    ofSetWindowShape( 1280, 720 );
    
//    setupGUI();
    
    videoGrabber.setDeviceID( 0 );
    videoGrabber.initGrabber( w, h );
    videoSource = &videoGrabber;
    
//    videoPlayer.loadMovie( "going to the store.mp4" );
//    videoPlayer.setLoopState( OF_LOOP_NORMAL );
//    videoPlayer.play();
//    if ( ((ofxUIToggle*)videoGUI->getWidget("MUTE"))->getValue() )
//        videoPlayer.setVolume( 0.0 );
    
    vfx1.init();
    vfx1.setVideoSource( &videoGrabber );
    
//    vfx2.init();
//    vfx2.setOpticalFlowEnabled( false );
//    vfx2.hideGUI();
//    vfx2.setVideoSource( &videoPlayer );
    
    vfx1.setColor( colors[colorIndex] );
//    vfx2.setColor( colors[colorIndex] );
    
    big = &vfx1;
//    small = &vfx2;
    
    videoFXExporter.setVideoFX( &vfx1 );
    
    // add some fake tweets to the crawl
    crawl.init();
    crawl.addCrawlItem( "aarontweets", "here is my message #shoutout" );
    crawl.addCrawlItem( "somedude", "#shoutout omg im on tv" );
    crawl.addCrawlItem( "nikefan", "i like shoes" );
    crawl.addCrawlItem( "test_user", "im testing stuff" );
    crawl.addCrawlItem( "hacker", "im hacking stuff" );
    crawl.addCrawlItem( "social_media_pro", "im tweeting stuff" );
    
    voteDisplay.init();
    voteDisplay.setTopics( "apples", "oranges" );
    
    overlayGUI = new ofxUISuperCanvas( "OVERLAYS", 0, 0, 200, 200 );
    overlayGUI->addToggle( "DRAW CRAWL", &crawl.visible );
    overlayGUI->addSlider( "CRAWL SPEED", 1.0, 10.0, &crawl.crawlSpeed );
    overlayGUI->addSpacer();
    overlayGUI->addToggle( "DRAW VOTING", &voteDisplay.visible );
    overlayGUI->addSlider( "VOTING MIN SCALE", 0.0, 1.0, &voteDisplay.minScale );
    overlayGUI->addSlider( "VOTING MAX SCALE", 0.0, 2.0, &voteDisplay.maxScale );
    
    vector<string> images;
    images.push_back( "* 1280x720-FeelTV-Logo-01.png" );
    images.push_back( "* dog.png" );
    overlayGUI->addToggle( "DRAW OVERLAY", &drawOverlayImage );
    overlayGUI->addSlider( "OVERLAY OPACITY", 0.0, 1.0, &overlayImageOpacity );
    overlayGUI->addDropDownList( "OVERLAY IMAGE", images );
    overlayImage.loadImage( images[0].substr(2,images[0].length()-2) );
    
    
    overlayGUI->setColorBack( ofColor::grey );
    overlayGUI->autoSizeToFitWidgets();
    overlayGUI->loadSettings( "GUI/overlay.xml" );
    ofAddListener( overlayGUI->newGUIEvent, this, &testApp::overlayGuiEvent );

    // STREAMER
    // --------------------------------------------------
#if USE_VIDEOSTREAMER
    int video_w = 640;
    int video_h = 360;

    if(!streamer.setup("streamer.xml", ofGetWidth(), ofGetHeight(), video_w, video_h)) {
      printf("error: cannot setup the streamer.\n");
      ::exit(EXIT_FAILURE);
    }

    if(!streamer.start()) {
      printf("error: cannot start the streamer.\n");
      ::exit(EXIT_FAILURE);
    }

    int numframes = streamer.getNumSamplesNeededForAudioEncoding();
    if(!numframes) {
      printf("error: invalid amount of sample for audio encoding: %d\n", numframes);
      ::exit(EXIT_FAILURE);
    }
    numframes /= 2; 

    sound_stream.listDevices();
    sound_stream.setup(this, 0, 2, 44100, numframes, 4);
#endif

    // MULTI STREAMER 
    // --------------------------------------------------
#if USE_MULTI_VIDEOSTREAMER
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
  size_t nsamples = nsize * nchannels;
  for(size_t i = 0; i < nsamples; ++i) {
    input[i] *= 32768.0f;
  }

  streamer.addAudio(input, nsize, nchannels);
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

void testApp::overlayGuiEvent( ofxUIEventArgs &e ) {
    string name = e.widget->getName();
    if ( name.substr(0,2) == "* " )
        overlayImage.loadImage( name.substr( 2, name.length()-2 ) );
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
    
    vfx1.update( videoGrabber.isFrameNew() );
//    vfx2.update( videoPlayer.isFrameNew() );
    
    crawl.update();
    voteDisplay.update();
}

//--------------------------------------------------------------
void testApp::draw(){

  if(streamer.wantsNewFrame()) {
    streamer.beginGrab();
      drawVideo();
    streamer.endGrab();
  }

  drawVideo();
}

void testApp::drawVideo() {
    
    big->draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
//    small->draw( ofGetWidth()*.75, ofGetHeight()*.75, ofGetWidth()*.25, ofGetHeight()*.25 );
    
    if ( drawOverlayImage ) {
        ofPushStyle();
        ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        ofSetColor( 255, (int)(255*overlayImageOpacity) );
        overlayImage.draw( 0, 0 );
        ofPopStyle();
    }
    
    crawl.draw();
    voteDisplay.draw();

}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if ( key == 91 ) {
        voteDisplay.addVote( voteDisplay.topic1 );
    }
    else if ( key == 93 ) {
        voteDisplay.addVote( voteDisplay.topic2 );
    }
    
    if ( key == 'k' ) {
        vfx1.reloadShaders();
//        vfx2.reloadShaders();
    }
    else if ( key == 'h' ) {
        big->hideGUI();
        videoFXExporter.exporterGUI->setVisible( false );
        overlayGUI->setVisible( false );
//        small->hideGUI();
    }
    else if ( key == '1' ) {
        big = &vfx1;
//        small = &vfx2;
        big->showGUI();
        videoFXExporter.exporterGUI->setVisible( true );
        overlayGUI->setVisible( true );
//        small->hideGUI();
    }
//    else if ( key == '2' ) {
//        big = &vfx2;
//        small = &vfx1;
//        big->showGUI();
//        small->hideGUI();
//    }
    else if ( key == 45 ) {
        if ( --colorIndex < 0 )
            colorIndex = 6;
        vfx1.setColor( colors[colorIndex] );
//        vfx2.setColor( colors[colorIndex] );
    }
    else if ( key == 61 ) {
        if ( ++colorIndex > 6 )
            colorIndex = 0;
        vfx1.setColor( colors[colorIndex] );
//        vfx2.setColor( colors[colorIndex] );
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
//    vfx2.exit();
//    videoGUI->saveSettings("GUI/video.xml");
    videoFXExporter.exporterGUI->saveSettings("GUI/exporter.xml");
    overlayGUI->saveSettings( "GUI/overlay.xml" );
}
