#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Crawl.h"
#include "VoteDisplay.h"
#include "TextOverlay.h"

class Overlay {
public:
    ofxUISuperCanvas * overlayGUI;
    
    TextOverlay textOverlay;
    VoteDisplay voteDisplay;
    Crawl crawl;
    
    ofImage overlayImage;
    bool drawOverlayImage = true;
    float overlayImageOpacity = 1.0;
    
    void init();
    void update();
    void draw();
    void overlayGuiEvent( ofxUIEventArgs &e );
    void disableGuiEvents();
    void drawGUI();
};

inline void Overlay::disableGuiEvents() {
  if(overlayGUI) {
    //overlayGUI->disableAppEventCallbacks();
    overlayGUI->disableAppDrawCallback();
  }
}

inline void Overlay::drawGUI() {
  if(overlayGUI) {
    overlayGUI->draw();
  }
}
