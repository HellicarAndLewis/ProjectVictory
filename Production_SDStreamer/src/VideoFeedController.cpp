#include "VideoFeedController.h"

#pragma mark - Helpers

// UI Button for the video device
class VideoDeviceToggle : public ofxUIToggle {
public:
    VideoDeviceToggle(float x, float y, bool v, string n):ofxUIToggle(x,y,v,n),usesMoviePlayer(false){
        //kind = KIND;
    }
    // const static int KIND = 1928;
    ofVideoDevice videoDevice;
    // Is this ineffienct?
    ofVideoGrabber videoGrabber;
    ofVideoPlayer videoPlayer;
    bool usesMoviePlayer;
};

typedef vector<VideoDeviceToggle*> VideoDeviceToggles;
typedef vector<VideoDeviceToggle*>::iterator VideoDeviceTogglesIt;

#pragma mark - Lifecycle

void VideoFeedController::init() {
    
    // Create the canvas
    
    gui = new ofxUISuperCanvas( "VIDEO SWITCHING", 220, 20, 200, 200 );
    
    // List and create UI for devices (excluding any facetime camera )
    
    ofVideoGrabber videoGrabber;
    devices = videoGrabber.listDevices();
    int i = 0;
    for ( ofVideoDevicesIt it = devices.begin(); it != devices.end(); ++it ) {
        
        if (ofToUpper(it->deviceName).find("FACETIME") == std::string::npos) {
            VideoDeviceToggle *videoToggle = new VideoDeviceToggle(20.0f, 20.0f, true, ofToUpper(ofToString(++i)+" "+ it->deviceName));
            
            cout << "Creating video device (id:" << ofToString((it)->id) << " name:" << (it->deviceName) << ")" << endl;
            
            videoToggle->videoDevice = *it;
            videoToggle->videoGrabber.setDeviceID(it->id);
            videoToggle->videoGrabber.initGrabber(640, 480);
            
            videoSource = &videoToggle->videoGrabber;
            
            gui->addWidgetDown(videoToggle);
            videoToggles.push_back(videoToggle);
        }

    }
    
    //get all the pngs in overlays
    ofDirectory dir( ofToDataPath("./videos/") );
    dir.allowExt("mov");
    dir.listDir();
    vector<ofFile>files = dir.getFiles();
    for (vector<ofFile>::iterator it=files.begin(); it!=files.end(); ++it) {
        
        // Add in the video
        VideoDeviceToggle *videoLooper = new VideoDeviceToggle(20.0f, 20.0f, true, ofToUpper(it->getFileName()));
        videoLooper->usesMoviePlayer = true;
        videoLooper->videoPlayer.loadMovie( "./videos/"+it->getFileName() );
        videoLooper->videoPlayer.play();
        gui->addWidgetDown(videoLooper);
        videoToggles.push_back(videoLooper);
        
        videoSource = &videoLooper->videoPlayer;
    }
    

    
    // Add a slider to change the time
    gui->addSlider(ofToUpper("Toggle Every Mins"), 0.01, 30.0f, &toggleEveryMins);
    gui->addLabelButton("TOGGLE NOW", false);
    
    ofAddListener( gui->newGUIEvent, this, &VideoFeedController::guiEvent );
    
    gui->loadSettings( "GUI/video.xml" );

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
            if ( (*currentVideoDeviceIt)->usesMoviePlayer ) {
                videoSource = &(*currentVideoDeviceIt)->videoPlayer;
            } else {
                videoSource = &(*currentVideoDeviceIt)->videoGrabber;
            }
        }
   
        lastSwitchTimeSecs = currentTime;
    }
    
    if (videoSource) {
        videoSource->update();
        videoSource->isFrameNew();
    }
}

#pragma mark - UI

void VideoFeedController::guiEvent(ofxUIEventArgs &e) {
    
    
    cout << "e: videoFeedController" << e.widget->getName() << endl;
    
    if ( e.widget->getName() == "TOGGLE NOW" && ((ofxUIButton*)e.widget)->getValue() ) {
        
        lastSwitchTimeSecs = -100000.0f;
        
    }
    
}