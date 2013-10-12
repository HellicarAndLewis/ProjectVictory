#pragma once

#include "ofMain.h"

class CrawlItem {
public:
    string username, message;
    ofVec2f pos;
    ofRectangle messageRect, usernameRect, boundingRect;
    CrawlItem( string username, string message, ofTrueTypeFont & usernameFont, ofTrueTypeFont & messageFont, float space ) {
        this->username = username;
        this->message = message;
        calculateRects( usernameFont, messageFont, space );
    }
    
    void calculateRects( ofTrueTypeFont & usernameFont, ofTrueTypeFont & messageFont, float space ) {
        messageRect = messageFont.getStringBoundingBox( message, 0, 0 );
        usernameRect = usernameFont.getStringBoundingBox( username, messageRect.getLeft() + messageRect.getWidth() + space, 0 );
        boundingRect = ofRectangle( messageRect );
        boundingRect.growToInclude( usernameRect );
    }
};

class Crawl {
public:
    
    float fontSize = 26 ;
    
    bool visible = true;
    ofTrueTypeFont usernameFont, messageFont;
    
    float spaceBetweenItems = 30.0;
    void setSpaceBetweenItems( float spaceBetweenItems ) {
        this->spaceBetweenItems = spaceBetweenItems;
    }
    
    float crawlSpeed = 5.0;
    void setCrawlSpeed ( float crawlSpeed ) {
        this->crawlSpeed = crawlSpeed;
    }
    
    int maxCrawlItems = 10;
    void setMaxCrawlItems( int maxCrawlItems ) {
        this->maxCrawlItems = maxCrawlItems;
    }
    
    int itemIndex = 0;
    vector<CrawlItem> items;
    vector<CrawlItem> screenItems;
    
    void init();
    void loadFonts();
    void addScreenItem();
    void update();
    void draw();
    void addCrawlItem( string username, string message );
};