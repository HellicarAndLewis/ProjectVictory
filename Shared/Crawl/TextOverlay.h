#pragma once

#include "ofMain.h"

class TextOverlay {
public:
    
    bool visible = false;
    float fontScale = 1.0;
    float lineSpacing = 20.0;
    string text = "test";
    ofVec2f pos;
    ofTrueTypeFont font;
    
    void init();
    void draw();
};