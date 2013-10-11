#include "SDOverlay.h"

void SDOverlay::init() {


    overlayGUI = new ofxUICanvas( ofGetWidth()-220, 20, 200, 200 );
    // text input for overlay text
    // Add the images
    vector<string> images;
    
    //get all the pngs in overlays
    ofDirectory dir( ofToDataPath("./overlays/") );
    dir.allowExt("png");
    dir.listDir();
    vector<ofFile>files = dir.getFiles();
    for (vector<ofFile>::iterator it=files.begin(); it!=files.end(); ++it) {
        images.push_back( "* " + it->getFileName() );
    }
    
    overlayGUI->addToggle( "DRAW OVERLAY", &drawOverlayImage );
    overlayGUI->addDropDownList( "OVERLAY IMAGE", images );
    
    overlayGUI->setColorBack( ofColor::grey );
    overlayGUI->autoSizeToFitWidgets();
    ofAddListener( overlayGUI->newGUIEvent, this, &SDOverlay::overlayGuiEvent );
    overlayGUI->loadSettings( "GUI/overlay.xml" );
}

void SDOverlay::update() { }

void SDOverlay::draw() {
    
    if ( drawOverlayImage ) {
        ofPushStyle();
        ofEnableBlendMode( OF_BLENDMODE_ALPHA );
        ofSetColor( 255, (int)(255*overlayImageOpacity) );
        overlayImage.draw( 0, 0 );
        ofPopStyle();
    }
}

void SDOverlay::overlayGuiEvent( ofxUIEventArgs &e ) {
    string name = e.widget->getName();
    if ( name.substr(0,2) == "* " ) {
        
        if ( ((ofxUIToggle *)e.widget)->getValue() ) {
            string filepath = ofToDataPath("./overlays/" + name.substr( 2, name.length()-2 ));
            if ( ofFile::doesFileExist( filepath ) ) {
                overlayImage.loadImage( filepath );
            }
        }
    }
}