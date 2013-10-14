#include "Crawl.h"

void Crawl::init() {
    loadFonts();
}

void Crawl::loadFonts() {
    usernameFont.loadFont( "NikeEnergy-Headline.ttf", fontSize );
    messageFont.loadFont( "IT646___.TTF", fontSize );
    for ( int i=0; i<items.size(); i++ ) {
        items[i].calculateRects( usernameFont, messageFont, ((int)fontSize) / 2.0 );
    }
    screenItems.clear();
}

void Crawl::addCrawlItem( string username, string message ) {
    
    CrawlItem item( "@" + ofToUpper( username ), message, usernameFont, messageFont, ((int)fontSize) / 2.0 );
    items.push_back( item );
    
    while ( items.size() > maxCrawlItems ) {
        items.erase( items.begin() );
    }
    
    // set item index to the newest one so we get it on screen asap
    itemIndex = items.size() - 1;
}

void Crawl::addScreenItem() {
    if ( items.size() == 0 )
        return;
    
    screenItems.push_back( items[ itemIndex ] );
    CrawlItem & item = *(screenItems.end()-1);
    item.pos.x = ofGetWidth();
    item.pos.y = ofGetHeight() + 12.0f;
    
    if ( ++itemIndex == items.size() )
        itemIndex = 0;
}

void Crawl::update() {
    
    if ( usernameFont.getSize() != (int)fontSize ) {
        cout << "loading fonts at " << fontSize << endl;
        loadFonts();
    }
    
    if ( screenItems.size() == 0 && items.size() == 0 )
        return;
    
    // if its empty, get it started
    if ( screenItems.size() == 0 ) {
        addScreenItem();
    }
    
    // scroll the screen items
    for ( int i=0; i<screenItems.size(); i++ ) {
        screenItems[i].pos.x -= crawlSpeed;
        // if the item is off screen, remove it from the screen items
        if ( screenItems[i].boundingRect.getRight() + screenItems[i].pos.x < 0 ) {
            screenItems.erase( screenItems.begin() + i );
            i--;
        }
    }
    
    // check to see if its time to add a new item
    CrawlItem & lastItem = *(screenItems.end()-1);
    if ( lastItem.boundingRect.getRight() + lastItem.pos.x + spaceBetweenItems < ofGetWidth() ) {
        addScreenItem();
    }
}

void Crawl::draw() {
    
    if ( !visible )
        return;
    
    for ( int i=0; i<screenItems.size(); i++ ) {
        
        CrawlItem & item = screenItems[i];
        messageFont.drawString( item.message, item.messageRect.getLeft() + item.pos.x, item.usernameRect.getTop() + item.pos.y );
        usernameFont.drawString( item.username, item.usernameRect.getLeft() + item.pos.x, item.usernameRect.getTop() + item.pos.y );
        
    }
}