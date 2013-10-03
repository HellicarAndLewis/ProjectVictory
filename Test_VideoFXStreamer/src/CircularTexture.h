#pragma once

#include "ofMain.h"

class CircularTexture {
    
    public:
        int width, height, depth;
        int offset;
    
//        ofTexture texture;
        ofTextureData texData;
    
        unsigned int textureID;
        int textureTarget;
    
        void allocate( int width, int height, int depth );
        void bind();
        void unbind();
        void addData( const void * data, int w, int h );
        void clear();
};
