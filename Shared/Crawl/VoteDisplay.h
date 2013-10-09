#pragma once

#include "ofMain.h"

class VoteDisplay {
public:
    
    ofTrueTypeFont font;
    ofVec2f pos;
    bool visible = true;
    
    float margin = 30.0;
    
    float minScale = .25;
    void setMinScale( float minScale ) {
        this->minScale = minScale;
    }
    
    float maxScale = 1.0;
    void setMaxScale( float maxScale ) {
        this->maxScale = maxScale;
    }
    
    string topic1 = "TOPIC1", topic2 = "TOPIC2";
    
    int topic1Votes = 0, topic2Votes = 0;
    
    void setTopics( string topic1, string topic2 ) {
        this->topic1 = ofToUpper( topic1 );
        this->topic2 = ofToUpper( topic2 );
        topic1Votes = 0;
        topic2Votes = 0;
    }
    
    void addVote( string topic );
    
    void init();
    void update();
    void draw();
    
    
};