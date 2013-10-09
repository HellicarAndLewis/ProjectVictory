#include "WebSystemController.h"
#include "Overlay.h"
#include "Crawl.h"

#pragma mark – Life Cycle

void WebSystemController::init() {
    // Connect the websystem
    connection.setHost("projectvictory.hostfile", 80);
    // Set up listeners
    connection.addHashTagCountListener(this, &WebSystemController::onHashTagCount);
    connection.addShoutoutListener(this, &WebSystemController::onShoutout);
    connection.addCommandListener(this, &WebSystemController::onCommand);
}

void WebSystemController::update() {
    float currentTime = ofGetElapsedTimef();
    static float lastHashTagUpdate = 0.f;
    if ( lastHashTagUpdate + (float(UPDATE_VOTES_EVERY_MS) / 1000) < currentTime && overlay ) {
        connection.requestHashTagCount( overlay->voteDisplay.topic1 );
        connection.requestHashTagCount( overlay->voteDisplay.topic2 );
        lastHashTagUpdate = currentTime;
    }
}

#pragma mark – Web System Connection

void WebSystemController::onHashTagCount(Json::Value body) {
    
    // Do something with the incoming count
    string hashTag = body["tag"].asString();
    // This will return the straight out number.
    // You can dig into  "result" to get an array with number of votes cast per minute for 120 minutes
    float numberOfShoutouts = WebSystem::Utils::countHashTagsInResult( body["result"] );
    
    overlay->voteDisplay.setVote(hashTag, numberOfShoutouts);
    
}

void WebSystemController::onShoutout(Json::Value body) {
    
    // Get the info on the shoutout (you might want to store these in a vector if they are used later)
    string screenname = body["tweet"]["userScreenName"].asString();
    string tweetText = body["tweet"]["text"].asString();
    
    cout << "@" << screenname << ": " << tweetText << endl;
    
    overlay->crawl.addCrawlItem( screenname, tweetText );
    
}

void WebSystemController::onCommand(Json::Value body) {
    
    // p.s. if you want to know what is the body, you can send it to cout:
    // cout << body;
    
    // Get the screen name of the user who tweeted
    string screenname = body["tweet"]["userScreenName"].asString();
    
    // DO NOT USE THE TWEET TEXT. It is unmoderated!
    string doNotUse = body["tweet"]["text"].asString();
    
    // Get the text command which trigger the action
    string command = body["payload"]["command"].asString();
    
    cout << "@" << screenname << " triggered " << command << endl;
    
    // Apply the payload to the videofx. They need to be a decayer somewhere, so this may not be how it works.
    if (videoFX) {
        WebSystem::Utils::applyPayload( videoFX, body["payload"] );
    }
    
}

#pragma mark – Decay

void WebSystemController::decayVideoFXToDefault() {
    
    // store the last time on first run (this won't work if using multiple effects)
    static float lastUpdate = ofGetElapsedTimef();
    
    // I think, the 0.3 means that each effect will decay by 30% every second
    WebSystem::Utils::decayEffects( videoFX, lastUpdate, 0.3f );
    
    lastUpdate = ofGetElapsedTimef();
    
}