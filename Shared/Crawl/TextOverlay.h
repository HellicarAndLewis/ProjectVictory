#pragma once

#include "ofMain.h"

class TextOverlay {
public:
    
    bool visible = false;
    float fontScale = 1.0;
    float lineSpacing = 20.0;
    float textOpacity;

    string text = "roxlu";
    ofVec2f pos;
    ofTrueTypeFont font;
    
    void init();
    void draw();
    void setTextOpacity(float o);
};


inline void TextOverlay::setTextOpacity(float o) {
  textOpacity = o;
}
