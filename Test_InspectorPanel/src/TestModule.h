#pragma once

#include "ofMain.h"
#include "ofxVMIG.h"

class TestModule : public ofxVMIG::Module {
public:
	void setup() override;
	void draw(float width, float height) override;
	void populateInspector(ofxUICanvas& canvas) override;
    void inspectorCallback(ofxUIEventArgs& args) override;
protected:
	ofImage image;
	bool selectNewImage;
	float alpha;
};