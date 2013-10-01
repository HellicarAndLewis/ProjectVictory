#include "Inspector.h"

using namespace ofxCvGui;

namespace ofxVMIG {
	//----------
	Inspector::Inspector() {
		this->widgets.disableMouseEventCallbacks();
		this->widgets.disableAppDrawCallback();
		this->widgets.disableWindowEventCallbacks();
		this->widgets.disableAppUpdateCallback();

		this->onUpdate += [this] (UpdateArguments&) {
			this->widgets.update();
		};
		this->onDraw += [this] (DrawArguments& args) {
			this->widgets.draw();
		};
		this->onMouse += [this] (MouseArguments& args) {
			this->mouse(args);
		};
		this->onBoundsChange += [this] (BoundsChangeArguments& args) {
			this->boundsChange(args);
		};

		ofAddListener(this->widgets.newGUIEvent, this, &Inspector::onUIChange);
		this->focused = 0;
	}

	//----------
	void Inspector::setFocus(Module& module) {
		this->clearFocus();
		this->widgets.addLabel("Inspector", "Inspector", 0);
		this->widgets.addSpacer();
		this->widgets.addSpacer();

		module.populateInspector(this->widgets);
		this->focused = &module;
	}

	//----------
	void Inspector::clearFocus() {
		this->widgets.removeWidgets();
		this->focused = 0;
	}

	//----------
	void Inspector::mouse(MouseArguments& args) {
		switch(args.action) {
		case MouseArguments::Pressed:
			this->widgets.mousePressed(args.local.x, args.local.y, args.button);
			break;
		case MouseArguments::Released:
			this->widgets.mouseReleased(args.local.x, args.local.y, args.button);
			break;
		case MouseArguments::Dragged:
			this->widgets.mouseDragged(args.local.x, args.local.y, args.button);
			break;
		case MouseArguments::Moved:
			this->widgets.mouseMoved(args.local.x, args.local.y);
			break;
		}
	}
	
	//----------
	void Inspector::boundsChange(BoundsChangeArguments& args) {
		this->widgets.setPosition(0,0);
		auto rect = this->widgets.getRect();
		rect->setWidth(args.bounds.width);
		rect->setHeight(args.bounds.height);
	}

	//----------
	void Inspector::onUIChange(ofxUIEventArgs& args) {
		if (this->focused != 0) {
			this->focused->inspectorCallback(args);
		}
	}
}