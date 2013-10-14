#include <ofxLinkDeck/ofxLinkDeck.h>
#include <iostream>
//#include <libyuv/libyuv.h>


ofxLinkDeck::ofxLinkDeck() 
  :pixel_format(0)
  ,display_mode(0)
  ,pixels(NULL)
  ,w(0)
  ,h(0)
  ,sws(NULL)
  ,has_new_frame(false)
{
  uv_mutex_init(&mutex);
}

ofxLinkDeck::~ofxLinkDeck() {
  pixel_format = 0;
  display_mode = 0;

  if(pixels) {
    delete[] pixels;
    pixels = NULL;
  }

}

#if 0
void ofxLinkDeck::setFrameCallback(decklink_capture_callback frameCB, void* userCB) {
  cb_frame = frameCB;
  cb_user = userCB;
}
#endif

bool ofxLinkDeck::setup(int device, BMDPixelFormat pixelFormat, BMDDisplayMode displayMode) {

  if(!dl.setup(device)) {
    printf("error: cannot setup the decklink device.\n");
    return false;
  }

  if(!dl.setCallback(ofx_linkdeck_frame, this)) {
    printf("error: cannot setup ofxLinkDeck, failed settings the frame callbabck.\n");
    return false;
  }

  pixel_format = pixelFormat;
  display_mode = displayMode;

  if(pixelFormat != bmdFormat8BitYUV) {
    printf("error: this addons has only tested bmdFormat8BitYUV.\n");
    return false;
  }

  if(!dl.setVideoMode(displayMode, pixelFormat)) {
    printf("error: cannot set the video mode.\n");
    return false;
  }

  w = dl.getWidth(display_mode);
  if(w < 0) {
    printf("error: cannot get the width for the current display mode, so we can't continue.\n");
    return false;
  }

  h = dl.getHeight(display_mode);
  if(h < 0) {
    printf("error: cannot get the height for the current display mode so we can't continue.\n");
    return false;
  }

  nbytes = w * h * 3;
  
  pixels = new uint8_t[nbytes];
  if(!pixels) {
    printf("error: cannot allocate the buffer for the converted image.\n");
    return false;
  }
  
  sws = sws_getContext(w, h, AV_PIX_FMT_UYVY422,
                       w, h, AV_PIX_FMT_RGB24, 
                       SWS_FAST_BILINEAR, NULL, NULL, NULL);
  if(!sws) {
    printf("error: cannot setup the sws conversion context.\n");
    return false;
  }

  return true; 
}

void ofxLinkDeck::listDevices() {
  dl.printDevices();
}

// --------------------------------------------

void ofx_linkdeck_frame(IDeckLinkVideoInputFrame* vframe, IDeckLinkAudioInputPacket* aframe, void* user) {
  //printf("frame callback.\n");

  if(!vframe) {
    printf("error:invalid vframe.\n");
    return;
  }

#if !defined(NDEBUG)
  if(!user) {
    printf("error: no user pointer set in the ofx_linkdeck_frame, which we need to validate the frame size for the memcpy/conversion.\n");
    return;
  }
#endif

  ofxLinkDeck* ld = static_cast<ofxLinkDeck*>(user);
  if(!ld) {
    printf("error: cannot cast the user pointer to ofxLinkDeck.\n");
    return;
  }

  uint32_t w = vframe->GetWidth();
  uint32_t h = vframe->GetHeight();
  uint32_t stride = vframe->GetRowBytes();
  uint8_t* uyvy422 = NULL;

#if !defined(NDEBUG)
  if(w != ld->w || h != ld->h) {
    printf("error: the input size is not the same as the one defined.\n");
    ::exit(EXIT_FAILURE);
  }
#endif

  HRESULT r = vframe->GetBytes((void**)&uyvy422);
  if(r != S_OK) {
    printf("error: cannot get yuv422 bytes.\n");
    return ;
  }

  uint64_t n = uv_hrtime();
  uv_mutex_lock(&ld->mutex);
  { 
    unsigned char* dest_planes[1] = { ld->pixels } ;
    int in_stride = stride;
    int dest_stride = ld->w * 3;
    int sr = sws_scale(ld->sws, 
                       (uint8_t**)&uyvy422, 
                       &in_stride,
                       0,
                       h, 
                       dest_planes, 
                       &dest_stride);
    //printf("%02X, %02X, %02X, %02X\n", ld->pixels[0], ld->pixels[1], ld->pixels[2], ld->pixels[2]);
    if(sr != h) {
      printf("error: the scaling went wrong...\n");
    }
    else {
      ld->has_new_frame = true;
    }

  }
  uv_mutex_unlock(&ld->mutex);
  uint64_t d = uv_hrtime() - n;
  //printf("took: %lld ns, %f ms to convert.\n", d, double(d)/1000000.0);

}
