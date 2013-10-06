#include "CircularTexture.h"

void CircularTexture::allocate( int width, int height, int depth ) {
    
    clear();
    
    offset = -1;
    this->width = width;
    this->height = height;
    this->depth = depth;
    
    
//    texture.texData.textureTarget = textureTarget = GL_TEXTURE_3D;
    texData.textureTarget = textureTarget = GL_TEXTURE_3D;
    
    // generate a texture
    glGenTextures( 1, (GLuint *)&textureID );
    texData.textureID = textureID;
    // set up the texture
    glEnable( textureTarget );
    glBindTexture( textureTarget, textureID );
//    texture.setUseExternalTextureID( textureID );
    texData.width = width;
    texData.height = height;
    texData.tex_h = width;
    texData.tex_w = height;
    // initialize texture to black
    glTexImage3D(
                 textureTarget,
                 0,
                 GL_RGB, // internal representation
                 (GLint)width,
                 (GLint)height,
                 (GLint)depth,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 0
                 );
    // set some params
    glTexParameterf( textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameterf( textureTarget, GL_TEXTURE_WRAP_R, GL_REPEAT );
    
    glBindTexture( textureTarget, 0 );
    glDisable( textureTarget );
}

void CircularTexture::addData( const void *data, int w, int h ) {
    if ( ++offset == depth )
        offset = 0;
    
////    if ( ++offset >= depth )
////        return;
//
//    cout << "adding some data at offset: " << offset << endl;
    
    glEnable( textureTarget );
    glBindTexture( textureTarget, textureID );
    glTexSubImage3D(
                    textureTarget,
                    0,
                    0,
                    0,
                    offset,
                    w,
                    h,
                    1,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
                    );
    
    glBindTexture( textureTarget, 0 );
    glDisable( textureTarget );
}

void CircularTexture::bind() {
    glEnable( textureTarget );
    glBindTexture( textureTarget, textureID );
}

void CircularTexture::unbind() {
    glBindTexture( textureTarget, 0 );
    glDisable( textureTarget );
}

void CircularTexture::clear() {
    glDeleteTextures( 1, &textureID );
}