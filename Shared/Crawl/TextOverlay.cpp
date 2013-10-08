#include "TextOverlay.h"

void TextOverlay::init() {
    font.loadFont( "NikeEnergy-Headline.ttf", 100, true, false, true );
    pos.set( ofGetWidth() / 2.0, ofGetHeight() / 2.0 );
}

void TextOverlay::draw() {
    
    if ( !visible )
        return;
    
    ofRectangle boundingRect = font.getStringBoundingBox( text, 0, 0 );
    ofRectangle heightRect = font.getStringBoundingBox( "1", 0, 0 );
    
    ofPushMatrix();
    ofTranslate( pos.x, pos.y );
    ofScale( fontScale, fontScale );
    ofTranslate( -boundingRect.getWidth()/2, heightRect.getHeight()/2 );
    font.drawString( text, 0, 0 );
    ofPopMatrix();
}