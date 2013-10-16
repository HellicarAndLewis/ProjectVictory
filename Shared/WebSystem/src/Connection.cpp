#include "Connection.h"

using namespace WebSystem;

#pragma mark - Connection management

void Connection::setHost(string host, unsigned int port) {
    _host = host;
    _port = port;
    // Connect
    if (_isConnected) { disconnect(); }
    connect();
}

void Connection::connect() {
    _shouldBeConnected = true;
    try {
    	wsClient.connect(_host, _port, false);
    } catch (...) {
        printf("catched exception in ofxLibwebsockets");
        _shouldBeConnected = false;
    }
}

void Connection::disconnect() {
    _shouldBeConnected = false;
    wsClient.close();
}

#pragma mark - Life cycle


void Connection::update() {
    
    // lock here? should just copy
    wsClient.lock();
    {
        std::copy(todo_messages.begin(), todo_messages.end(), std::back_inserter(work_messages));
        todo_messages.clear();
    }
    wsClient.unlock();
    
    for (vector<Json::Value>::iterator it = work_messages.begin(); it != work_messages.end(); ++it) {
        
        Json::Value json = *it;

        // A new commands message
        if ( json["resource"] == "/command/new/" ) {
            // Trigger all the listeners
            for (CallablesIt it = commandListeners.begin(); it != commandListeners.end(); ++it) {
                (**it)( json["body"] );
            }
        }
        // A new shoutout message
        else if ( json["resource"] == "/shoutout/new/" ) {
            // Trigger all the listeners
            for (CallablesIt it = shoutoutListeners.begin(); it != shoutoutListeners.end(); ++it) {
                (**it)( json["body"] );
            }
        }
        // Check for a count
        else if ( json["resource"] == "/hashtags/count/new/" ) {
            // See if the count is for us
            if ( !json["token"] ) { return; }
            if ( hasToken( json["token"].asString(), true) ) {
                // Trigger all the listeners
                for (CallablesIt it = hashTagCountListeners.begin(); it != hashTagCountListeners.end(); ++it) {
                    (**it)( json["body"] );
                }
            }
        }
    }
    
    work_messages.clear();
    
    
}

#pragma mark - Requests

string Connection::requestHashTagCount(string tag, hashTagResolution resolution) {
    string resolutionStr = "";
    switch (resolution) {
        case HASH_TAG_MINUTE: { resolutionStr = "minute"; break; }
        case HASH_TAG_HOUR: { resolutionStr = "hour"; break; }
        case HASH_TAG_DAY: { resolutionStr = "day"; break; }
        case HASH_TAG_WEEK: { resolutionStr = "week"; break; }
        default: { ofLogError() << "WebSystem::requestHashTagCount Invalid resolution"; return; break; }
    }
    
    string token = "not connected";
    
    if (_isConnected) {
    
        // Create the request
        Json::Value jsonRequest;
        token = getToken();
        jsonRequest["resource"] = "/hashtags/preformcount/"+tag+"/"+resolutionStr+"/";
        jsonRequest["token"] = token;
        
        // Get request as a string
        Json::StyledWriter writer;
        string message = writer.write( jsonRequest );
        wsClient.send( message );
    }

    return token;
}

string Connection::getToken() {
    string token = ofToString( ofRandomf(), 6 ) + ofToString(ofGetElapsedTimef(), 4);
    tokens.push_back(token);
    return token;
}

bool Connection::hasToken(string token, bool removeIfFound) {
    bool found = find(tokens.begin(), tokens.end(), token)!=tokens.end();
    if (found && removeIfFound) {
        removeToken(token);
    }
    return found;
}

void Connection::removeToken(string token) {
    tokens.erase(std::remove(tokens.begin(), tokens.end(), token), tokens.end());
}

#pragma mark - Websocket delegates

void Connection::onConnect( ofxLibwebsockets::Event& args ) {
    // cout << "ws onConnect" << endl;
}

void Connection::onOpen( ofxLibwebsockets::Event& args ) {
    _isConnected = true;
    // cout << "ws onOpen" << endl;
}

void Connection::onClose( ofxLibwebsockets::Event& args ) {
    _isConnected = false;
    // cout << "ws onClose" << endl;
}

void Connection::onIdle( ofxLibwebsockets::Event& args ) {
    // cout << "ws onIdle" << endl;
}

void Connection::onMessage( ofxLibwebsockets::Event& args ) {

    Json::Value json = args.json;
    
    todo_messages.push_back(json);
    
}

void Connection::onBroadcast( ofxLibwebsockets::Event& args ) {
    // cout << "ws onBroadcast" << endl;
}

#pragma mark - Check connection

void Connection::onUpdate(ofEventArgs &data) {
    // If not connected, wait 3 seconds and attempt a reconnection
    static bool isAttemptingReconnection = false;
    static float shouldAttemptReconnectionAt = 0.f;
    if (!_isConnected && _shouldBeConnected && !isAttemptingReconnection) {
        isAttemptingReconnection = true;
        shouldAttemptReconnectionAt = ofGetElapsedTimef() + WEBSYSTEM_RECONNECT_EVERY_SECS;
    }
    if (isAttemptingReconnection && shouldAttemptReconnectionAt < ofGetElapsedTimef()) {
        isAttemptingReconnection = false;
        if (!_isConnected) {
            wsClient.connect(_host, _port, false);
        }
    }
}
