#ifndef __CALIBRATION_APP_H__
#define __CALIBRATION_APP_H__

#include <memory>

#include <ofMain.h>

#include "position_taker.h"


template <class T> class CalibrationApp : public ofBaseApp {
private:
	T* controller;
	std::shared_ptr< PositionTaker<T> > taker;
	int phase;
	ofPoint screen[4];
	bool done;

	void setPositionTaker(){
		static const ofPoint schedule[] = {
			ofPoint(0, 0),
			ofPoint(1, 0),
			ofPoint(1, 1),
			ofPoint(0, 1)
		};

		ofPoint pos(
			schedule[phase].x * ofGetWidth(),
			schedule[phase].y * ofGetHeight()
		);

		taker = std::shared_ptr< PositionTaker<T> >(new PositionTaker<T>(controller, pos));
	}

public:
	CalibrationApp(T* controller) : controller(controller) { }

	void setup() override {
		ofSetWindowTitle("calibrator");
		ofSetFrameRate(30);
		ofBackground(ofColor::black);

		phase = 0;
		setPositionTaker();
		done = false;
	}

	void update() override {
		if(!done){
			taker->update();

			if(taker->isDone()){
				screen[phase].set(taker->result);

				if(phase == 3){
					controller->setScreen(screen);
					phase = 0;
					done = true;
				}else{
					phase++;
					setPositionTaker();
				}
			}
		}
	}

	void draw() override {
		taker->draw();
	}

	void keyPressed(const int key) override {
		if(key == ' '){
			phase = 0;
			setPositionTaker();
			done = false;
		}
	}
};


#endif
