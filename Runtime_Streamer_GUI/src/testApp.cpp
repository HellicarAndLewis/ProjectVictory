#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  ofBackground(0x33);
  ofSetWindowPosition(310,190);
  ofSetWindowTitle("Victory GUI");
  gui.setup("remoteing.xml", true);
  gui.show();
  gui.load();
}

//--------------------------------------------------------------
void testApp::update(){
	gui.update();
}


//--------------------------------------------------------------
void testApp::draw(){
  gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

  // handled by gui
  if(gui.onKeyPressed(key)) {
    return;
  }

	if(key == ' ') {
		gui.toggle();
	}
  else if(key == 's') {
    gui.save();
  }
  else if(key == 'l') {
    gui.load();
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
