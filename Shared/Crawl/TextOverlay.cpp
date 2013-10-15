#include "TextOverlay.h"

void TextOverlay::init() {
  textOpacity = 1.0f;
  font.loadFont( "NikeEnergy-Headline.ttf", 100, true, false, true );
  pos.set( ofGetWidth() / 2.0, ofGetHeight() / 2.0 );
}

void TextOverlay::draw() {
    
  if(!visible){
    return;
  }

  ofSetColor(255, 255, 255, textOpacity * 255);

  // break text up into individual lines
  string delimiter = ",";
  vector<string> lines = ofSplitString( text, delimiter );
  vector<ofRectangle> boundingRects;

  // get individual bounding rects
  float maxWidth = 0;
  for ( int i=0; i<lines.size(); i++ ) {
    
    boundingRects.push_back( font.getStringBoundingBox( lines[i], 0, 0 ) );
    maxWidth = (boundingRects.end()-1)->getWidth() > maxWidth 
      ? (boundingRects.end()-1)->getWidth() 
      : maxWidth;
  }
  pos.set( ofGetWidth() / 2.0, ofGetHeight() / 2.0 );
  ofRectangle heightRect = font.getStringBoundingBox( "1", 0, 0 );
  ofPushMatrix();
  {
    ofTranslate( pos.x, pos.y );
    ofScale( fontScale, fontScale );
    if ( lines.size() > 1 ) {
      ofTranslate( 0, -(lines.size() * heightRect.getWidth() + (lines.size()-1) * lineSpacing)/2.0 - heightRect.getWidth() );
    }
    else {
      ofTranslate( 0, heightRect.getWidth() );
    }

    // draw individual lines
    for(int i = 0; i < lines.size(); i++){
      ofPushMatrix();
      {
        float font_x = -((maxWidth) / 2);
        float font_y = (heightRect.getHeight()+lineSpacing) * i;
        ofTranslate(font_x, font_y);
        font.drawString(lines[i], 0, 0);
      }
      ofPopMatrix();
    }
  }
  ofPopMatrix();

  ofSetColor(255, 255, 255, 255);
}
