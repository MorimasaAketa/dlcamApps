#pragma once

#define HOST "localhost" // ��M����IP�A�h���X
#define PORT 8000 // ��M���̃|�[�g�ԍ�

#include "ofMain.h"
#include "ofxTurboJpeg.h"

class ofxZmqSubscriber;
class ofxZmqPublisher;

class controlApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	ofxZmqSubscriber* subscriber;
	ofxZmqPublisher* publisher;

	ofBuffer send_buf;

	ofFbo fbo, mainFbo;
	ofxTurboJpeg turbo;
	ofPixels pix720;
	ofTexture tex720, tex2160;
	int64_t img_count;
	bool isFrameNew;
	bool shouldWrite;
	bool ackWaiting;
	int timeLastSent;
	int timeoutMillis = 3000 ;

};


