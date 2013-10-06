#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // Connect the websystem
    connection.setHost("146.185.139.19", 80);
    // Set up listeners
    connection.addHashTagCountListener(this, &testApp::onHashTagCount);
    connection.addShoutoutListener(this, &testApp::onShoutout);
    connection.addCommandListener(this, &testApp::onCommand);
    
    // Set up the video effects
    webcam.initGrabber( ofGetWidth(), ofGetHeight() );
    vfx.init();
    vfx.setVideoSource( &webcam );
    
    // Give it some sane defaults
    vfx.badTVEffect.enabled = true;
    vfx.badTVEffect.applyShaderMap(vfx.badTVEffect.getShaderDefaultMap());
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Request a count of #shoutout 10 seconds after the app has started up
    static bool hasRequestedShoutout = false;
    if (!hasRequestedShoutout && ofGetElapsedTimef() > 10.0f) {
        // Get the hash tags
        connection.requestHashTagCount("shoutout");
        hasRequestedShoutout = true;
    }
    
    // Vfx
    webcam.update();
    vfx.update( webcam.isFrameNew() );
    
}

//--------------------------------------------------------------
void testApp::draw(){
    // Video Effects
    vfx.draw( ofGetWidth(), 0, -ofGetWidth(), ofGetHeight() );
    
    // Renderer
    shoutoutRenderer.draw();
    hashtagRenderer.draw();
    
}

#pragma mark - Web System eventss

void testApp::onHashTagCount(Json::Value body) {
    
    // Do something with the incoming count
    hashtagRenderer.addResult( body );
}

void testApp::onShoutout(Json::Value body) {
    
    // Do something with the shoutout
    shoutoutRenderer.addTweet( body["tweet"] );
    
}

void testApp::onCommand(Json::Value body) {
    
    // Apply the payload to the videofx. They need to be a decayer somewhere, so this may not be how it works.
    WebSystem::Utils::applyPayload(&vfx, body["payload"]);
    
}

