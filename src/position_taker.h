#ifndef __POSITION_TAKER_H__
#define __POSITION_TAKER_H__

#include <ofMain.h>
#include <ofxCv.h>

#define WAIT	20


template <class T> class PositionTaker {
private:
	T* controller;
	int phase, timer;
	ofPoint pos;
	ofImage pre;

	static ofPoint findDiffPoint(ofImage& pre, ofImage& aft) {
		ofImage diff;

		ofxCv::absdiff(pre, aft, diff);

		int max_ = 0;
		ofPoint maxPoint;
		auto pixels = diff.getPixels();
		for(int y=0; y<diff.getHeight()/8; y++){
			for(int x=0; x<diff.getWidth()/8; x++){
				int score = 0;
				for(int ys=0; ys<8; ys++){
					for(int xs=0; xs<8; xs++){
						score += pixels[x*8+xs + (y*8+ys)*diff.getWidth()];
					}
				}
				if(score > max_){
					max_ = score;
					maxPoint.set(x*8+4, y*8+4);
				}
			}
		}

		return maxPoint;
	}

public:
	ofPoint result;

	PositionTaker(T* controller, ofPoint pos) {
		this->controller = controller;
		this->pos = pos;

		timer = phase = 0;
	}

	void update() {
		if(timer == WAIT/2){
			if(phase == 0){
				pre = *controller->getRawImage();
				ofxCv::GaussianBlur(pre, pre, 16);
			}else if(phase == 1){
				ofImage aft = *controller->getRawImage();
				ofxCv::GaussianBlur(aft, aft, 16);
				result.set(findDiffPoint(pre, aft));
			}
		}

		timer++;
		if(timer > WAIT && phase <= 1){
			phase++;
			timer = 0;
		}
	}

	void draw() const {
		if(phase == 1){
			ofSetColor(ofColor::white);
			ofDrawCircle(pos, 20);
		}
	}

	bool isDone() const {
		return phase >= 2 && timer > WAIT;
	}
};


#endif
