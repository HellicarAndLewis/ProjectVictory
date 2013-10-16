#include "WebSystemController.h"
#include "Overlay.h"
#include "Crawl.h"
#include "StreamerGUI.h"

#pragma mark – Life Cycle

void WebSystemController::init() {
    // Connect the websystem
    connection.setHost("projectvictory.hostfile", 80);
    // Set up listeners
    connection.addHashTagCountListener(this, &WebSystemController::onHashTagCount);
    connection.addShoutoutListener(this, &WebSystemController::onShoutout);
    connection.addCommandListener(this, &WebSystemController::onCommand);
    
    // Set up the voting GUI
    //initVotingGUI();
    //initWebSystemGUI();
}

void WebSystemController::update() {
    
    connection.update();
    
    float currentTime = ofGetElapsedTimef();
    
    static float lastHashTagUpdate = 0.f;
    if ( lastHashTagUpdate + (float(UPDATE_VOTES_EVERY_MS) / 1000) < currentTime && overlay && webSystemIsEnabled && countHashTags ) {
        connection.requestHashTagCount( overlay->voteDisplay.topic1 );
        connection.requestHashTagCount( overlay->voteDisplay.topic2 );
        lastHashTagUpdate = currentTime;
    }
    
    // only decay effects 10/sec
    static float lastEffectsDecaySecs = 0.0f;
    if ( shouldDecaysEffects && currentTime - lastEffectsDecaySecs > DECAY_EFFECT_EVERY_SEC) {
        decayVideoFXToDefault();
        lastEffectsDecaySecs = currentTime;
    }
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
    
    // Apply the payload to the videofx. They need to be a decayer somewhere, so this may not be how it works.
    if (videoFX) {
        WebSystem::Utils::applyPayload( videoFX, body["payload"], effectImpact );
        
        bool foundActiveEffect = false;
        
        // Loop through each effect found in the pay load
        for (int i = 0;  i<body["payload"]["effects"].size(); ++i) {
            // Loop though each VideoFX effect
            for (vector<BaseEffect*>::iterator it = videoFX->effects.begin(); it!= videoFX->effects.end(); ++it) {
                // See it is matches
                Json::Value effects;
                try {
                    effects = body["payload"]["effects"]; //[i]["name"].asString();
                } catch (...) {
                    cout << "unable to parse effects from payload json" << endl;
                    continue;
                }
                // Check to see if there is an effect enabled
                for (int j = 0; j < effects.size(); ++j) {
                    if (effects[j]["name"].asString() == (*it)->name) {
                        if ((*it)->enabled) {
                            foundActiveEffect = true;
                        }
                    }
                }
            }
        }
        
        if ( foundActiveEffect ) {
            
            float currentTime = ofGetElapsedTimef();
            
            if ( currentTime - lastScreenshotSavedAt > (60.0f/MAX_IMAGES_PER_MINUTE) ) {
                string tweetid;
                try {
                    // Get the tweet id as a string
                    stringstream s;
                    s << body["tweet"]["id"];

                    // Strip out all the non decimal values
                    string tweetid_tmp = s.str();
                    std::string tweetid = "";
                    for(size_t i = 0; i < tweetid_tmp.size(); ++i) {
                      if(isdigit(tweetid_tmp[i])) {
                        tweetid.push_back(tweetid_tmp[i]);
                      }
                    }

                    // Set it up to be saved
                    screenShotTriggers[ ofGetElapsedTimef() ] = tweetid;
                } catch (...) {
                    cout << "error: failed to capture tweet id" << endl;
                }
                
                lastScreenshotSavedAt = currentTime;
            } 
            
            // Bump up the alpha for any commands
            // If we go back to multiple effetcs per command this
            // will display effects overlays even if they are now active
            string payloadName;
            try {
                payloadName = body["payload"]["payloadName"].asString();
            } catch (...) {
                cout << "unable to parse payload name from json" << endl;
                return;
            }
            if (gui) {
                for ( std::vector<CommandOverlayImage*>::iterator it = gui->command_overlay_images.begin(); it != gui->command_overlay_images.end(); ++it ) {
                    if ((*it)->commandName == payloadName) {
                        (*it)->opacity = 1.0f;
                    }
                }
            }
        }
        
    }
    
}

#pragma mark – Decay

void WebSystemController::decayVideoFXToDefault() {
    
    // store the last time on first run (this won't work if using multiple effects)
    static float lastUpdate = ofGetElapsedTimef();
    
    // decayRate: I think, the 0.3 means that each effect will decay by 30% every second
    WebSystem::Utils::decayEffects( videoFX, lastUpdate, decayRate );
    
    lastUpdate = ofGetElapsedTimef();
    
}

// Image saving
string WebSystemController::getNextScreenShotFilename() {

  if(shouldTriggerFakeScreenshot) {
    shouldTriggerFakeScreenshot = false;

    ofDirectory dir(ofToDataPath("./screenshots/"));
    dir.allowExt("jpg");
    dir.listDir();
    vector<ofFile> files = dir.getFiles();

    char buf[512];
    sprintf(buf, "fake_%04ld", files.size() + 1);
    return buf;
  }


    float currentTime = ofGetElapsedTimef();
    string filename = "";
    
    // find the first trigger needing to be saved
    ScreenShotTriggersIt it = screenShotTriggers.begin();
    
    bool found = false;
    while ( !found && it != screenShotTriggers.end()) {
        if ( it->first < currentTime + 5.0f) {
            found = true;
            filename = it->second;
            screenShotTriggers.erase(it);
        }
    }
    
    return filename;
    
}


