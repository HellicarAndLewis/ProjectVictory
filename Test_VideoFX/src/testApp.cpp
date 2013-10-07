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
    overlayGUI->addSlider( "FONT SIZE", 8.0, 64.0, &crawl.fontSize );
    overlayGUI->addSlider( "CRAWL SPEED", 1.0, 20.0, &crawl.crawlSpeed );
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
    
    big->draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    
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
    }
    else if ( key == 'h' ) {
        big->hideGUI();
        videoFXExporter.exporterGUI->setVisible( false );
        overlayGUI->setVisible( false );
    }
    else if ( key == '1' ) {
        big = &vfx1;
        big->showGUI();
        videoFXExporter.exporterGUI->setVisible( true );
        overlayGUI->setVisible( true );
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
    overlayGUI->saveSettings( "GUI/overlay.xml" );
}
