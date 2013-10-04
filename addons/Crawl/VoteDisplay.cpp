#include "VoteDisplay.h"

void VoteDisplay::init() {
    font.loadFont( "NikeEnergy-Headline.ttf", 100, true, false, true );
    pos.set( ofGetWidth()/2, ofGetHeight()/2 );
}

void VoteDisplay::update() {
    
}

void VoteDisplay::addVote( string topic ) {
    if ( ofToUpper(topic) == topic1 ) {
        topic1Votes++;
    }
    else if ( ofToUpper(topic) == topic2 ) {
        topic2Votes++;
    }
}

void VoteDisplay::draw() {
    
    
    
    float halfHeight = font.getStringBoundingBox( "1", 0, 0 ).getHeight() / 2.0;
    
    float totalVotes = topic1Votes + topic2Votes;
    float topic1Scale = ofMap( (totalVotes==0?.5:topic1Votes/totalVotes), 0.0, 1.0, minScale, maxScale );
    float topic2Scale = ofMap( (totalVotes==0?.5:topic2Votes/totalVotes), 0.0, 1.0, minScale, maxScale );
    
    ofRectangle topic1Rect = font.getStringBoundingBox( topic1, 0, 0 );
    ofPushMatrix();
    ofTranslate( pos.x , pos.y );
    ofScale( topic1Scale, topic1Scale );
    ofTranslate( - (topic1Rect.getRight() + margin ), halfHeight );
    font.drawString( topic1, 0, 0 );
    ofPopMatrix();
    
    ofRectangle topic2Rect = font.getStringBoundingBox( topic2, 0, 0 );
    ofPushMatrix();
    ofTranslate( pos.x, pos.y );
    ofScale( topic2Scale, topic2Scale );
    ofTranslate( 0, halfHeight );
    font.drawString( topic2, margin, 0 );
    ofPopMatrix();
}