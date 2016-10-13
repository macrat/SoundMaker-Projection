#ifndef __MAIN_APP_H__
#define __MAIN_APP_H__

#include <memory>

#include <ofMain.h>
#include <ofxCv.h>

#include "config.h"
#include "note_effect.h"


template <class T> class MainApp : public ofBaseApp {
private:
	T* controller;
	std::vector< std::shared_ptr<NoteEffect> > effects;
	ofImage bg;
	bool notes[NOTES_WIDTH][NOTES_HEIGHT];
	ofSoundStream sound;
	unsigned int phase;
	int takeBGTimer;

public:
	MainApp(T* controller) : controller(controller) { }

	void setup() override;
	void update() override;
	void draw() override;

	void keyPressed(const int key) override;

	void audioOut(ofSoundBuffer& buffer);
};


template <class T> void MainApp<T>::setup() {
	ofSetWindowTitle("SoundMaker");
	ofBackground(0, 0, 0);

	ofSoundStreamSettings settings;

#ifdef TARGET_LINUX
	const auto devices = sound.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setOutDevice(devices[0]);
	}
#endif

	settings.setOutListener(this);
	settings.sampleRate = SAMPLE_RATE;
	settings.numOutputChannels = 1;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;

	sound.setup(settings);

	takeBGTimer = -1;
}


template <class T> void MainApp<T>::update() {
	if(controller->isFrameNew()){
		ofImage img;

		ofxCv::GaussianBlur(*controller->getImage(), img, BLUR);
		ofxCv::absdiff(img, bg, img);
		img.update();

		ofPixels pixels = img.getPixels();
		const int xbase = (int)((ofGetElapsedTimeMillis() % MEASURE / (float)MEASURE + 0.1) * NOTES_WIDTH) % NOTES_WIDTH;
		for(int y=0; y<NOTES_HEIGHT; y++){
			for(int x=xbase; x<(xbase+NOTES_WIDTH/4)%NOTES_WIDTH; x++){
				int score = 0;
				for(int ys=0; ys<RESOLUTION; ys++){
					for(int xs=0; xs<RESOLUTION; xs++){
						score += pixels[x*RESOLUTION/2+xs + (y*RESOLUTION+ys)*CAM_WIDTH];
					}
				}
				notes[x][y] = score > THRESHOLD;
			}
		}


		if(takeBGTimer >= 0){
			takeBGTimer++;

			if(takeBGTimer > 10){
				ofxCv::GaussianBlur(*controller->getImage(), bg, BLUR);
				takeBGTimer = -1;
			}
		}
	}

	const int x = ofGetElapsedTimeMillis() % MEASURE / (float)MEASURE * NOTES_WIDTH;
	for(int y=0; y<NOTES_HEIGHT; y++){
		for(int xs=-SOUND_RANGE; xs<=SOUND_RANGE; xs++){
			if(notes[(x + xs)%NOTES_WIDTH][y]){
				effects.push_back(std::shared_ptr<NoteEffect>(new NoteEffect((x + xs)%NOTES_WIDTH, y)));
			}
		}
	}

	ofRemove(effects, NoteEffect::shouldRemove);
}


template <class T> void MainApp<T>::draw() {
	if(takeBGTimer < 0){
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		for(auto e: effects){
			e->draw();
		}
		ofDisableBlendMode();

		ofSetColor(255, 255, 255);
		const int x = (ofGetElapsedTimeMillis() % MEASURE / (float)MEASURE) * ofGetWidth();
		ofDrawLine(x, 0, x, ofGetHeight());
	}
}


template <class T> void MainApp<T>::keyPressed(const int key) {
	if(key == ' '){
		takeBGTimer = 0;
	}
}


template <class T> void MainApp<T>::audioOut(ofSoundBuffer& buffer) {
	const int x = (ofGetElapsedTimeMillis() % MEASURE / (float)MEASURE) * NOTES_WIDTH;

	if(takeBGTimer < 0){
		for(unsigned int i=0; i<buffer.getNumFrames(); i++){
			phase++;

			buffer[i] = 0;

			for(int y=0; y<NOTES_HEIGHT; y++){
				for(int xs=-SOUND_RANGE; xs<=SOUND_RANGE; xs++){
					if(notes[(x+xs)%NOTES_WIDTH][y]){
						buffer[i] += sin(phase * ((1-((double)y/NOTES_HEIGHT))*(MAX_FREQ-MIN_FREQ) + MIN_FREQ) * 2*PI / SAMPLE_RATE) * VOLUME;
					}
				}
			}
		}
	}
}


#endif
