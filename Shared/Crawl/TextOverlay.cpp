#include "TextOverlay.h"

void TextOverlay::init() {
    font.loadFont( "NikeEnergy-Headline.ttf", 100, true, false, true );
    pos.set( ofGetWidth() / 2.0, ofGetHeight() / 2.0 );
}

void TextOverlay::draw() {
    
    if ( !visible )
        return;
    
    // break text up into individual lines
    string delimiter = ",";
    vector<string> lines = ofSplitString( text, delimiter );
    vector<ofRectangle> boundingRects;
    // get individual bounding rects
    float maxWidth = 0;
    for ( int i=0; i<lines.size(); i++ ) {
        boundingRects.push_back( font.getStringBoundingBox( lines[i], 0, 0 ) );
        maxWidth = (boundingRects.end()-1)->getWidth() > maxWidth ? (boundingRects.end()-1)->getWidth() : maxWidth;
    }
    
//    ofRectangle boundingRect = font.getStringBoundingBox( text, 0, 0 );
    ofRectangle heightRect = font.getStringBoundingBox( "1", 0, 0 );
    
    ofPushMatrix();
    ofTranslate( pos.x, pos.y );
    ofScale( fontScale, fontScale );
    if ( lines.size() > 1 )
        ofTranslate( 0, -(lines.size() * heightRect.getWidth() + (lines.size()-1) * lineSpacing)/2.0 - heightRect.getWidth() );
    else
        ofTranslate( 0, heightRect.getWidth() );
    // draw individual lines
    for ( int i=0; i<lines.size(); i++ ) {
        ofPushMatrix();
        ofTranslate( -maxWidth/2, (heightRect.getHeight()+lineSpacing) * i );
        font.drawString( lines[i], 0, 0 );
        ofPopMatrix();
    }
//    ofTranslate( -boundingRect.getWidth()/2, heightRect.getHeight()/2 );
//    font.drawString( text, 0, 0 );
    ofPopMatrix();
}