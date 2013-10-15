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
    void loadAndShowImage(ofFile& f);

    void setTextEnabled(bool n);
    void setDrawOverlayImage(bool v);
    void setOverlayImageOpacity(float f);
    void setCrawEnabled(bool e);
    void setCrawlSpeed(float s);
    void setTextScale(float s);
    void setTextSpacing(float s);
    void setTextOpacity(float o);
};

inline void Overlay::disableGuiEvents() {
  /*
  if(overlayGUI) {
    //overlayGUI->disableAppEventCallbacks();
    overlayGUI->disableAppDrawCallback();
  }
  */
}

inline void Overlay::drawGUI() {
  /*
  if(overlayGUI) {
    overlayGUI->draw();
  }
  */
}

inline void Overlay::setTextEnabled(bool e) {
  textOverlay.visible = e;
}

inline void Overlay::setDrawOverlayImage(bool v) {
  drawOverlayImage = v;
}

inline void Overlay::setOverlayImageOpacity(float f) {
  if(f < 0) {
    f = 0.0f;
  }
  else if(f > 1.0) {
    f = 1.0f;
  }
  overlayImageOpacity = f;
}

inline void Overlay::setTextOpacity(float o) {
  textOverlay.setTextOpacity(o);
}

inline void Overlay::setCrawEnabled(bool e) {
  crawl.visible = e;
}

inline void Overlay::setCrawlSpeed(float s) {
  crawl.setCrawlSpeed(s);
}

inline void Overlay::setTextScale(float s) {
  textOverlay.fontScale = s;
}

inline void Overlay::setTextSpacing(float s) {
  textOverlay.lineSpacing = s;
}
