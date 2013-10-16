#pragma once
#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "Callable.h"

#define WEBSYSTEM_RECONNECT_EVERY_SECS 3.0f

namespace WebSystem {
    
    // Maintains the connection to the server. Will re-attempt a connection every 5 seconds.
    // Responsible for all communication with the server via sockets

    class Connection {
    public:
        
        Connection():
        _isConnected(false),
        _shouldBeConnected(false){
            wsClient.addListener(this);
            ofAddListener(ofEvents().update, this, &Connection::onUpdate);
        }
        
        virtual ~Connection() {
            ofRemoveListener(ofEvents().update, this, &Connection::onUpdate);
        }
        
        void update();
        
        // Connection management
        void setHost(string host, unsigned int port = 80);
        void isConnected() { return _isConnected; }
        
        // Requests
        
        enum hashTagResolution {
            HASH_TAG_MINUTE,
            HASH_TAG_HOUR,
            HASH_TAG_DAY,
            HASH_TAG_WEEK
        };
        
        // This will get the server to send out the number of hashtags, returns a token
        string requestHashTagCount(string tag, hashTagResolution resolution = HASH_TAG_MINUTE);
        
        // Websocket handler
        ofxLibwebsockets::Client wsClient;
        
        // Websocket delegates
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );
        
        // Check connection
        void onUpdate(ofEventArgs &data);
        
        // Listeners
        // Usage: `connection.addHashTagCountListener(this, &testApp::onHashTagCount);`
    
        template <class Object>
        void addHashTagCountListener(Object* obj, void (Object::*MemFun)(Json::Value body)) {
            hashTagCountListeners.push_back(new TemplateCallable<Object>(obj, MemFun));
        }
        
        template <class Object>
        void addShoutoutListener(Object* obj, void (Object::*MemFun)(Json::Value body)) {
            shoutoutListeners.push_back(new TemplateCallable<Object>(obj, MemFun));
        }
        
        template <class Object>
        void addCommandListener(Object* obj, void (Object::*MemFun)(Json::Value body)) {
            commandListeners.push_back(new TemplateCallable<Object>(obj, MemFun));
        }
        
    protected:
        
        // Connection management
        
        void connect();
        void disconnect();
        string _host;
        unsigned int _port;
        bool _isConnected;
        bool _shouldBeConnected;
        
        // Requests
        // Tokens
        vector<string> tokens;
        string getToken();
        bool hasToken(string token, bool removeIfFound = false);
        void removeToken(string token);
        
        // Listeners
        vector<Callable*> hashTagCountListeners;
        vector<Callable*> shoutoutListeners;
        vector<Callable*> commandListeners;
        
        // Store messages comming in form ofxLibWebsockets
        vector<Json::Value> work_messages;
        vector<Json::Value> todo_messages;

    };

    typedef vector<Callable*> Callables;
    typedef vector<Callable*>::iterator CallablesIt;

}