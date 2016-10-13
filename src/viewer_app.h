#ifndef __VIEWER_APP_H__
#define __VIEWER_APP_H__

#include <ofMain.h>


template <class T> class ViewerApp : public ofBaseApp {
private:
	T* controller;

public:
	ViewerApp(T* controller) : controller(controller) { }

	void setup() override {
		ofSetWindowTitle("viewer");
	}

	void draw() override {
		controller->getImage()->draw(0, 0, ofGetWidth(), ofGetHeight());
	}
};


#endif
