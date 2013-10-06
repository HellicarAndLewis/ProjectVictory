#pragma once

#include "ofMain.h"
#include "VideoFX.h"
#include "json.h"
class EffectMapToggle;

// Exports the shader uniforms to JSON file
// To be used to create preset for sending to the VMIG cue via the websystem.
//
// Example: in testApp::setup vfxExporter.setVideoFX(&vfx1);

class VideoFXExporter {
public:

	VideoFXExporter():videoFx(0){}
	virtual ~VideoFXExporter(){}

    // Sets the pointer to the active VideoFX
	void setVideoFX(VideoFX *vfx) { videoFx = vfx; createGUI(); }

	// GUI
    void createGUI();
	void guiEvent(ofxUIEventArgs &e);
	ofxUISuperCanvas *exporterGUI;
    map<string, map<string, EffectMapToggle *> > effectToggles;
    
    // Saving
    static string exportDirectory;
    enum exportType {
        EXPORT_ALL,
        EXPORT_SELECTED,
        EXPORT_DEFAULT,
    };
    void saveToFile(string payloadName, exportType type = EXPORT_ALL, string command = "");

	// JSON Utils
    // Gets json effect maps for all BaseEffects
	Json::Value getEffectsAsJson(string payloadName, exportType type = EXPORT_ALL);
	Json::Value getEffectAsJson(BaseEffect *effect, exportType type = EXPORT_ALL);
    string getJsonAsString(Json::Value json);

protected:
    
    ofxUITextInput * payloadNameTextInput;
    ofxUITextInput * commandTextInput;
	VideoFX *videoFx;

};

// toggle button
class EffectMapToggle : public ofxUIToggle {
public:
    EffectMapToggle(string effectName, string mapName)
    :effectName(effectName),mapName(mapName),
    ofxUIToggle(10.0f, 10.0f, true, ofToUpper(mapName), OFX_UI_FONT_SMALL){};
    string effectName;
    string mapName;
};