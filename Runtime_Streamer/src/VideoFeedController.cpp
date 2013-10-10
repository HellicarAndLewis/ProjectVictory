#include "VideoFeedController.h"

#pragma mark - Helpers

// UI Button for the video device
class VideoDeviceToggle : public ofxUIToggle {
public:
    VideoDeviceToggle(float x, float y, bool v, string n):ofxUIToggle(x,y,v,n){
        kind = KIND;
    }
    const static int KIND = 1928;
    ofVideoDevice videoDevice;
    // Is this ineffienct?
    ofVideoGrabber videoGrabber;
};

typedef vector<VideoDeviceToggle*> VideoDeviceToggles;
typedef vector<VideoDeviceToggle*>::iterator VideoDeviceTogglesIt;

#pragma mark - Lifecycle

void VideoFeedController::init() {
    
    // Create the canvas
    
    gui = new ofxUISuperCanvas( "VIDEO SWITCHING", 500, 0, 200, 200 );
    
    // List and create UI for devices
    
    ofVideoGrabber videoGrabber;
    devices = videoGrabber.listDevices();
    int i = 0;
    for ( ofVideoDevicesIt it = devices.begin(); it != devices.end(); ++it ) {
        VideoDeviceToggle *videoToggle = new VideoDeviceToggle(20.0f, 20.0f, true, ofToUpper(ofToString(++i)+" "+ it->deviceName));
        
        cout << "Creating video device (id:" << ofToString((it)->id) << " name:" << (it->deviceName) << ")" << endl;
        
        videoToggle->videoDevice = *it;
        videoToggle->videoGrabber.setDeviceID(it->id);
        videoToggle->videoGrabber.initGrabber(1280, 720);
        
        videoSource = &videoToggle->videoGrabber;
        
        gui->addWidgetDown(videoToggle);
        videoToggles.push_back(videoToggle);
    }
    
    // Add a slider to change the time
    gui->addSlider(ofToUpper("Toggle Every Mins"), 0.01, 30.0f, &toggleEveryMins);
    gui->addLabelButton("TOGGLE NOW", false);
    ofAddListener( gui->newGUIEvent, this, &VideoFeedController::guiEvent );
    

    gui->autoSizeToFitWidgets();

}

void VideoFeedController::update() {
    
    
    float currentTime = ofGetElapsedTimef();
    
    // Switch between devices
    static VideoDeviceTogglesIt currentVideoDeviceIt = videoToggles.begin();
   
    if (currentTime - lastSwitchTimeSecs > (toggleEveryMins * 60)) {
        
        // Find the next active toggle button
        VideoDeviceTogglesIt possibleNewVideoDeviceIt = currentVideoDeviceIt + 1;
        bool found = false;
        while ( !found && possibleNewVideoDeviceIt != currentVideoDeviceIt ) {
            if ( possibleNewVideoDeviceIt >= videoToggles.end() ) {
                
                possibleNewVideoDeviceIt = videoToggles.begin();
            } else if ( (*possibleNewVideoDeviceIt)->getValue() ) {
                found = true;
            } else {
                ++possibleNewVideoDeviceIt;
            }
        }
        
        if (found) {
            currentVideoDeviceIt = possibleNewVideoDeviceIt;
            videoSource = &(*currentVideoDeviceIt)->videoGrabber;
        }
   
        lastSwitchTimeSecs = currentTime;
    }
    
    
    videoSource->update();
    videoSource->isFrameNew();
}

#pragma mark - UI

void VideoFeedController::guiEvent(ofxUIEventArgs &e) {
    
    if ( e.widget->getName() == "TOGGLE NOW" && ((ofxUIButton*)e.widget)->getValue() ) {
        
        cout << "toggling now" << endl;
        
        lastSwitchTimeSecs = -100000.0f;
        
    }
    
}