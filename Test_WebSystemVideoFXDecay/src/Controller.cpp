#include "Controller.h"
#include "VideoFX.h"

#pragma mark - Setup

void Controller::init() {
    
    // Connect the websystem
    connection.setHost("146.185.139.19", 80);
    // Set up listeners
    connection.addHashTagCountListener(this, &Controller::onHashTagCount);
    connection.addShoutoutListener(this, &Controller::onShoutout);
    connection.addCommandListener(this, &Controller::onCommand);
    
}

#pragma mark – Web System Connection

void Controller::onHashTagCount(Json::Value body) {
    
    // Do something with the incoming count
}

void Controller::onShoutout(Json::Value body) {
    
    // Do something with the shoutout
    
}

void Controller::onCommand(Json::Value body) {
    // Apply the payload to the videofx. They need to be a decayer somewhere, so this may not be how it works.
    if (videoFX) {
        WebSystem::Utils::applyPayload( videoFX, body["payload"] );
    }
    
}

#pragma mark – Decay

void Controller::decayVideoFXToDefault() {
    
    static float lastUpdate = ofGetElapsedTimef();
    
    // cout << defaults << endl;
    
    WebSystem::Utils::decayEffects(videoFX, lastUpdate, 0.3f);
    
    lastUpdate = ofGetElapsedTimef();
    
}