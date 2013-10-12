/*

---------------------------------------------------------------------------------
      
                                               oooo              
                                               `888              
                oooo d8b  .ooooo.  oooo    ooo  888  oooo  oooo  
                `888""8P d88' `88b  `88b..8P'   888  `888  `888  
                 888     888   888    Y888'     888   888   888  
                 888     888   888  .o8"'88b    888   888   888  
                d888b    `Y8bod8P' o88'   888o o888o  `V88V"V8P' 
                                                                 
                                                  www.roxlu.com
                                             www.apollomedia.nl  
                                          www.twitter.com/roxlu
              
---------------------------------------------------------------------------------

  # ofxLinkDeck 

  There are a couple of other Black Magic Decklinks but from what I read 
  those were only Windows... I had to get this working on Mac in at most
  "no time". So this code isnt heavily tested but it seems to work prety well.

  Make sure that if you use this and you want to get the pixels from the decklink
  they are converted from UYVY422 to RGB24 (this is not efficient but it had to 
  work like this for the project I created this for). 

  Also the frame data is given to used by the decklink sdk in a separate thread,
  therefore you need to call 'lock()' and 'unlock()' when you retrieve the 
  pixels.

  EXAMPLE:
  See: https://gist.github.com/roxlu/6941174#file-testapp-cpp-L29-L34

  IMPORTANT: 
  - call lock()/unlock() when retrieving pixels
  - call resetHasNewFrame()

 */
#ifndef ROXLU_OFX_LINK_DECK_H
#define ROXLU_OFX_LINK_DECK_H

#include <decklink/DeckLink.h>
#include <stdint.h>
#include "ofMain.h"

extern "C" {
#  include <libswscale/swscale.h>  
#  include <uv.h>
}

void ofx_linkdeck_frame(IDeckLinkVideoInputFrame* vframe, IDeckLinkAudioInputPacket* aframe, void* user);

class ofxLinkDeck {
 public:
  ofxLinkDeck();
  ~ofxLinkDeck();

  bool setup(int device, BMDDisplayMode displayMode, BMDPixelFormat pixelFormat); 

  void listDevices();        /* list the available decklinks */
  bool start();              /* start getting the video data */
  bool hasNewFrame();        /* check if there is a new frame in the pixels member; make sure to call resetHasNewFrame() when you grabbed them */
  void resetHasNewFrame();   /* call this to reset the "has_new_frame" flag */

  int getWidth();
  int getHeight();

  void lock();                /* we're getting the pixels from another thread so need to lock/unlock ths pixels member */
  void unlock();              /* "" */
  unsigned char* getPixels(); /* MAKE SURE TO LOCK() AND UNLOCK() around getPixels() */

 private:
  DeckLink dl;                 /* this does all the work with the decklink sdk */
  BMDPixelFormat pixel_format; /* the used decklink pixel format */
  BMDDisplayMode display_mode; /* the used display mode */

 public:
  uint8_t* pixels;          /* pixels that stores the video */
  uint32_t nbytes;          /* number of bytes in pixels */
  int w;                    /* width of the captured frame */
  int h;                    /* height o fhte captured frame */
  SwsContext* sws;          /* used to convert the incoming uyvy422 stream to rgb24*/
  bool has_new_frame;       /* is set to "true" when we get a new frame from the decklink sdk, is set in ofx_linkdeck_frame */
  uv_mutex_t mutex;         /* mutex used to protect the ofImge + has_new_frame */
};

inline bool ofxLinkDeck::start() {
  return dl.start();
}

inline void ofxLinkDeck::resetHasNewFrame() {
  uv_mutex_lock(&mutex);
    has_new_frame = false;
  uv_mutex_unlock(&mutex);
}

inline bool ofxLinkDeck::hasNewFrame() {
  uv_mutex_lock(&mutex);
  bool h = has_new_frame;
  uv_mutex_unlock(&mutex);
  return h;
}

inline int ofxLinkDeck::getWidth() {
  return w;
}

inline int ofxLinkDeck::getHeight() {
  return h;
}

inline void ofxLinkDeck::lock() {
  uv_mutex_lock(&mutex);
}

inline void ofxLinkDeck::unlock() {
  uv_mutex_unlock(&mutex);
}

// make sure to call lock() / unlock()
inline unsigned char* ofxLinkDeck::getPixels() {
  return (unsigned char*)pixels;
}

#endif
