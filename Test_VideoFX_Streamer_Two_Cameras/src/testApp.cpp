#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAntiAliasing();
    ofSetVerticalSync( true );
    ofSetFrameRate( 60.0 );
    ofSetLogLevel( OF_LOG_WARNING );
    
    w = 1280;
    h = 720;
    
    /* [verbose] ofQTKitGrabber: listing video devices:
     [verbose] ofQtKitGrabber: 0 - HD Pro Webcam C920 #2
     [verbose] ofQtKitGrabber: 1 - HD Pro Webcam C920
     [verbose] ofQtKitGrabber: 2 - Built-in iSight
     [verbose] ofQTKitGrabber: attached video device: HD Pro Webcam C920 #2
     [verbose] ofQTKitGrabber: starting video session*/
    
    // 1. Create a new recorder object.  ofPtr will manage this
    // pointer for us, so no need to delete later.
    cameras = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
    // 2. Set our video grabber to use this source.
    videoGrabber.setGrabber(cameras);
    // 3. Make lists of video devices.
    videoDevices = cameras->listVideoDevices();
    // 4. Init the grabber at the correct size
    videoGrabber.initGrabber(1280, 720);
    
    vfx1.init();
    vfx1.setVideoSource( &videoGrabber );
    
    vfx1.setColor( colors[colorIndex] );
    
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
}

void testApp::overlayGuiEvent( ofxUIEventArgs &e ) {
    string name = e.widget->getName();
    if ( name.substr(0,2) == "* " )
        overlayImage.loadImage( name.substr( 2, name.length()-2 ) );
}

//--------------------------------------------------------------
void testApp::update(){
    videoGrabber.update();
    
    vfx1.update( videoGrabber.isFrameNew() );
    
    crawl.update();
    voteDisplay.update();
}

//--------------------------------------------------------------
void testApp::draw(){
  if(streamer.hasNewFrame()) {
    streamer.beginGrab();
      drawVideo();
    streamer.endGrab();
	
  }
 drawVideo();

}

void testApp::drawVideo() {
    
    vfx1.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    
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
    
    switch (key) {
        case 91:
            voteDisplay.addVote( voteDisplay.topic1 );
            break;
        case 93:
            voteDisplay.addVote( voteDisplay.topic2 );
            break;
        case 'v':
            switchToOtherCamera();
            break;
        case 'k':
            vfx1.reloadShaders();
            break;
        case 'h':
            vfx1.hideGUI();
            videoFXExporter.exporterGUI->setVisible( false );
            overlayGUI->setVisible( false );
            break;
        case '1':
            vfx1.showGUI();
            videoFXExporter.exporterGUI->setVisible( true );
            overlayGUI->setVisible( true );
        case '-':
            if ( --colorIndex < 0 )
                colorIndex = 6;
            vfx1.setColor( colors[colorIndex] );
            break;
        case '=':
            if ( ++colorIndex > 6 )
                colorIndex = 0;
            vfx1.setColor( colors[colorIndex] );
            break;
        default:
            break;
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

void testApp::exit() {
    vfx1.exit();
    videoFXExporter.exporterGUI->saveSettings("GUI/exporter.xml");
    overlayGUI->saveSettings( "GUI/overlay.xml" );
}

void testApp::switchToOtherCamera(){
    if(cameras->getDeviceID() == 0){ //isight is always the last in the list, so this is the best worst way
        cameras->setDeviceID(1);
    }else{
        cameras->setDeviceID(0);
    }
}
