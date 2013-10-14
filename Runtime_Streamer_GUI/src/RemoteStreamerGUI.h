#ifndef ROXLU_REMOTE_STREAMER_GUI_H
#define ROXLU_REMOTE_STREAMER_GUI_H

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"

class RemoteStreamerGUI {
 public:
  RemoteStreamerGUI();
  ~RemoteStreamerGUI();
  bool setup();
  void update();
  void draw();
 public:
  void onOverlayAClicked();
  void onOverlayBClicked();

 public:
  /* osc */
  ofxOscSender sender;

  /* overlay */
  ofxPanel overlay_panel;
  ofxButton overlay_button;
  std::string overlay_text;

};

#endif
