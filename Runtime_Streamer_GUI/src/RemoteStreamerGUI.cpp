#include "RemoteStreamerGui.h"

RemoteStreamerGUI::RemoteStreamerGUI() {
}

RemoteStreamerGUI::~RemoteStreamerGUI() {
}

bool RemoteStreamerGUI::setup() {

  overlay_panel.setup("Overlay", "ofxgui/overlay_panel.xml", 10,10);
  overlay_panel.add(overlay_button.setup("Set overlay text"));

  overlay_button.addListener(this, &RemoteStreamerGUI::onOverlayAClicked);

  sender.setup("127.0.0.1", 4455);
  return true;
}

void RemoteStreamerGUI::update() {
}

void RemoteStreamerGUI::draw() {
  overlay_panel.draw();
}


// ----------------------------------------
void RemoteStreamerGUI::onOverlayAClicked() {
  overlay_text = ofSystemTextBoxDialog("Please enter the text for overlay 1 (comma creates new lines)");

  ofxOscMessage m;
  m.setAddress("/overlay/text");
  m.addStringArg(overlay_text);

  sender.sendMessage(m);
}

void RemoteStreamerGUI::onOverlayBClicked() {
  printf("b.\n");
}

