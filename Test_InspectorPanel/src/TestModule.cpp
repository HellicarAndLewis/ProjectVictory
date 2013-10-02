#include "TestModule.h"

//----------
void TestModule::setup() {
	this->selectNewImage = false;
	this->alpha = 255.0f;
}

//----------
void TestModule::draw(float width, float height) {
	this->image.draw(0,0,width, height);
	ofDrawBitmapString("test", 20, 20);
}

//----------
void TestModule::populateInspector(ofxUICanvas& canvas) {
	canvas.addButton("Select new image", &selectNewImage);
	canvas.addSlider("Alpha", 0.0f, 255.0f, &alpha);
}

//----------
void TestModule::inspectorCallback(ofxUIEventArgs& args) {
	if (args.getName() == "Select new image") {
		if (this->selectNewImage) {
			auto result = ofSystemLoadDialog("Select an image");
			image.loadImage(result.getPath());
		}
	}
}
