#include <decklink/DeckLinkCard.h>
#include <iostream>

DeckLinkCard::DeckLinkCard(IDeckLink* dl)
  :dl(dl)
  ,input(NULL)
  ,display_mode(bmdModeUnknown)
  ,pixel_format(bmdFormat8BitYUV)
  ,capture_callback(NULL)
  ,is_started(false)
  ,cb_user(NULL)
  ,cb_frame(NULL)
{

  if(!dl) {
    printf("error: create an DeckLinkCard instance with an invalid IDeckLink.\n");
    ::exit(EXIT_FAILURE);
  }

  dl->AddRef();

  HRESULT r = dl->QueryInterface(IID_IDeckLinkInput, (void**)&input);
  if(r != S_OK) {
    printf("error: cannot query the IID_IDeckLinkInput for the given IDeckLink. are you sure this device can capture?\n");
    ::exit(EXIT_FAILURE);
  }
}

DeckLinkCard::~DeckLinkCard() {
  
  cb_user = NULL;
  cb_frame = NULL;
  
  if(is_started) {
    stop();
  }

  if(dl) {
    dl->Release();
  }
  dl = NULL;

  if(input) {
    input->Release();
  }
  input = NULL;

  if(capture_callback) {
    delete capture_callback;
  }
  capture_callback = NULL;

  is_started = false; 
}

bool DeckLinkCard::initialize() {
  return true;
}

bool DeckLinkCard::isVideoModeSupported(BMDDisplayMode mode, BMDPixelFormat format) {

  if(!input) {
    printf("error: checking if a video mode is supported, but the card variable is invalid.\n");
    return false;
  }

  BMDDisplayModeSupport support;
  IDeckLinkDisplayMode* found_mode = NULL;
  
  HRESULT r = input->DoesSupportVideoMode(mode, format, bmdVideoInputFlagDefault, &support, &found_mode);
  if(r != S_OK) {
    printf("error: checking if the video- mode/pixel-format is supported failed.\n");
    return false;
  }

  found_mode->Release();
  found_mode = NULL;

  return support == bmdDisplayModeSupported;
}

int DeckLinkCard::getWidth(BMDDisplayMode mode) {

  if(!input) {
    printf("error: cannot get the width for the current capture card because the `input` member is not initialized.\n");
    return -1;
  }

  IDeckLinkDisplayMode* dmode = NULL;
  IDeckLinkDisplayModeIterator* mit = NULL;
  HRESULT hr = input->GetDisplayModeIterator(&mit);

  if(hr != S_OK) {
    printf("error: cannot get a display mode iterator.\n");
    return false;
  }

  int w = -1;

  while(mit->Next(&dmode) == S_OK) {

    if(dmode->GetDisplayMode() == mode) {
      w = dmode->GetWidth();
    }

    dmode->Release();
    dmode = NULL;

    if(w > 0) {
      break;
    }
  }

  mit->Release();
  mit = NULL;
  return w;
}

int DeckLinkCard::getHeight(BMDDisplayMode mode) {

  if(!input) {
    printf("error: cannot get the width for the current capture card because the `input` member is not initialized.\n");
    return -1;
  }

  IDeckLinkDisplayMode* dmode = NULL;
  IDeckLinkDisplayModeIterator* mit = NULL;
  HRESULT hr = input->GetDisplayModeIterator(&mit);

  if(hr != S_OK) {
    printf("error: cannot get a display mode iterator.\n");
    return false;
  }

  int h = -1;

  while(mit->Next(&dmode) == S_OK) {

    if(dmode->GetDisplayMode() == mode) {
      h = dmode->GetHeight();
    }

    dmode->Release();
    dmode = NULL;

    if(h > 0) {
      break;
    }
  }

  mit->Release();
  mit = NULL;
  return h;
}

bool DeckLinkCard::setVideoMode(BMDDisplayMode mode, BMDPixelFormat format) {

  if(!isVideoModeSupported(mode, format)) {
    printf("error: the given display mode and pixel format combination is invalid.\n");
    return false;
  }

  display_mode = mode;
  pixel_format = format;

  return true;
}

bool DeckLinkCard::start() {

  if(is_started) {
    printf("error: cannot start the capture because we're already captureing. Call stop() first.\n");
    return false;
  }

  if(display_mode == bmdModeUnknown) {
    printf("error: you did not yet specify the video mode and pixel format. Make sure to call setVideoMode().\n");
    return false;
  }

  if(!capture_callback) {
    capture_callback = new DeckLinkCaptureCallback();
  }

  capture_callback->setCallback(cb_frame, cb_user);

  HRESULT hr = input->SetCallback(capture_callback);
  if(hr != S_OK) {
    printf("error: cannot set the callback on the capture card.\n");
    return false;
  }

  hr = input->EnableVideoInput(display_mode, pixel_format, 0);
  if(hr != S_OK) {
    printf("error: cannot enable video input.\n");
    return false;
  }

  hr = input->StartStreams();
  if(hr != S_OK) {
    printf("error: cannot start the capture stream(s).\n");
    return false;
  }

  is_started = true;

  return true;
}

bool DeckLinkCard::stop() {

  if(!is_started) {
    printf("error: cannot stop captureing because we didn't start yet.\n");
    return false;
  }

  is_started = false;

  HRESULT hr = input->StopStreams();
  if(hr != S_OK) {
    printf("error: StopStreams() returned an error.\n");
    return false;
  }

  return true;
}
