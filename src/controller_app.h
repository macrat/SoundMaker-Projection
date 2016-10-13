#ifndef __CONTROLLER_APP_H__
#define __CONTROLLER_APP_H__

#include <memory>

#include <ofMain.h>
#include <ofxCv.h>

#include "calibration_app.h"
#include "config.h"
#include "main_app.h"
#include "viewer_app.h"


class ControllerApp : public ofBaseApp {
private:
	ofVideoGrabber cam;
	ofImage raw, img;
	std::vector<cv::Point2f> screen;
	std::weak_ptr<ofBaseApp> child;

	void createApp(std::shared_ptr<ofBaseApp> app) {
		ofGLFWWindowSettings settings;
		settings.width = 640;
		settings.height = 480;
		settings.resizable = true;
		ofRunApp(ofCreateWindow(settings), app);
	}

	void createChildApp(std::shared_ptr<ofBaseApp> app) {
		createApp(app);
		child = std::weak_ptr<ofBaseApp>(app);
	}

public:
	void setup() override {
		ofSetWindowTitle("controller");
		cam.setup(CAM_WIDTH, CAM_HEIGHT);

		createChildApp(std::shared_ptr<ofBaseApp>(new CalibrationApp<ControllerApp>(this)));
		createApp(std::shared_ptr<ofBaseApp>(new ViewerApp<ControllerApp>(this)));
	}

	void update() override {
		if(child.expired()){
			createChildApp(std::shared_ptr<ofBaseApp>(new MainApp<ControllerApp>(this)));
		}

		cam.update();
		if(cam.isFrameNew()){
			ofxCv::convertColor(cam, raw, CV_RGB2GRAY);

			if(screen.size() == 4){
				ofxCv::unwarpPerspective(raw, img, screen);
			}else{
				img = raw;
			}

			raw.update();
			img.update();
		}
	}

	void draw() override {
		ofSetColor(ofColor::white);
		raw.draw(0, 0, ofGetWidth(), ofGetHeight());

		ofSetColor(ofColor::green);
		for(auto p: screen){
			ofDrawCircle(p.x * ofGetWidth() / CAM_WIDTH, p.y * ofGetHeight() / CAM_HEIGHT, 8);
		}
	}

	void exit() override {
		std::exit(0);
	}

	std::shared_ptr<ofImage> getImage() const {
		return std::shared_ptr<ofImage>(new ofImage(img));
	}

	std::shared_ptr<ofImage> getRawImage() const {
		return std::shared_ptr<ofImage>(new ofImage(raw));
	}

	bool isFrameNew() const {
		return cam.isFrameNew();
	}

	void setScreen(const ofPoint screen[4]) {
		this->screen.clear();
		for(int i=0; i<4; i++){
			this->screen.push_back(cv::Point2f(screen[i].x, screen[i].y));
		}
	}
};


#endif
