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
    
    // Set up the voting GUI
    initVotingGUI();
    initWebSystemGUI();
}

void WebSystemController::update() {
    float currentTime = ofGetElapsedTimef();
    static float lastHashTagUpdate = 0.f;
    if ( lastHashTagUpdate + (float(UPDATE_VOTES_EVERY_MS) / 1000) < currentTime && overlay && webSystemIsEnabled ) {
        connection.requestHashTagCount( overlay->voteDisplay.topic1 );
        connection.requestHashTagCount( overlay->voteDisplay.topic2 );
        lastHashTagUpdate = currentTime;
    }
}

#pragma mark - VoteSystem

void WebSystemController::initWebSystemGUI() {
    websystemGUI = new ofxUISuperCanvas( "WEB SYSTEM", 400, 0, 200, 200 );
    websystemGUI->addLabelToggle( "ENABLED", &webSystemIsEnabled );
    websystemGUI->addLabelToggle( "SHOUTOUTS", &shoutoutsAreEnabled );
    websystemGUI->addLabelToggle( "COMMANDS", &commandsAreEnabled );
    websystemGUI->autoSizeToFitWidgets();
}

#pragma mark – Web System Connection

void WebSystemController::onHashTagCount(Json::Value body) {
    
    if (!webSystemIsEnabled) { return; }
    
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
    
    if (!webSystemIsEnabled || !shoutoutsAreEnabled) { return; }
    
    cout << "@" << screenname << ": " << tweetText << endl;
    
    overlay->crawl.addCrawlItem( screenname, tweetText );
    
}

void WebSystemController::onCommand(Json::Value body) {
    
    if (!webSystemIsEnabled || !commandsAreEnabled) { return; }
    
    // p.s. if you want to know what is the body, you can send it to cout:
    // cout << body;
    
    // Get the screen name of the user who tweeted
    string screenname = body["tweet"]["userScreenName"].asString();
    
    // DO NOT USE THE TWEET TEXT. It is unmoderated!
    string doNotUse = body["tweet"]["text"].asString();
    
    // Get the text command which trigger the action
    string command = body["payload"]["command"].asString();
    
    cout << "@" << screenname << " triggered " << command << endl;
    
    // Store a trigger to save the image
    
    // This is dirty. Want tweet id as string.
    stringstream s;
    s << body["tweet"]["id"];
    string tweetid = s.str();
    tweetid.erase(tweetid.size() - 1);
    screenShotTriggers[ ofGetElapsedTimef() ] = tweetid;
    
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

#pragma mark - VoteSystem

void WebSystemController::initVotingGUI() {
    voteGUI = new ofxUISuperCanvas( "VOTEING SYSTEM", 300, 0, 200, 200 );
    voteGUI->setColorBack(ofColor(ofColor::green, 125));
    voteGUI->addLabel("TOPIC 1");
    vote1TextInput = voteGUI->addTextInput("TOPIC 1", "");
    voteGUI->addLabel("TOPIC 2");
    vote2TextInput = voteGUI->addTextInput("TOPIC 2", "");
    voteGUI->addButton("UPDATE TOPICS", false);
    voteGUI->autoSizeToFitWidgets();
    ofAddListener( voteGUI->newGUIEvent, this, &WebSystemController::voteingGUIEvent );
    voteGUI->loadSettings("settings.voting.gui.xml");

}

void WebSystemController::voteingGUIEvent(ofxUIEventArgs &e) {
    
    if ( e.widget->getName() == "UPDATE TOPICS") {
        ofxUIButton *button = (ofxUIButton *)e.widget;
        if (button->getValue()) {
            overlay->voteDisplay.topic1 = vote1TextInput->getTextString();
            overlay->voteDisplay.topic2 = vote2TextInput->getTextString();
            overlay->voteDisplay.resetVotes();
        } else {
            voteGUI->saveSettings("settings.voting.gui.xml");
        }
    }
    
}

// Image saving
string WebSystemController::getNextScreenShotFilename() {
    float currentTime = ofGetElapsedTimef();
    string filename = "";
    
    // find the first trigger needing to be saved
    ScreenShotTriggersIt it = screenShotTriggers.begin();
    
    bool found = false;
    while ( !found && it != screenShotTriggers.end()) {
        
        if ( it->first < currentTime + 1.0f) {
            found = true;
            filename = it->second;
            screenShotTriggers.erase(it);
        }
        
    }
    
    return filename;
    
}

