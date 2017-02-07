#pragma once

#define HOST "localhost" // 受信側のIPアドレス
#define PORT 8000 // 受信側のポート番号

#define DLNJOINTS 19

#include "ofMain.h"
#include "ofxTurboJpeg.h"
#include "ofxJSON.h"

class ofxZmqSubscriber;
class ofxZmqPublisher;

class dlBody {
public:
	vector<ofVec3f> joints;
	ofVec2f upperLeft;
	ofVec2f lowerRight;
	void setup() 
	{
		joints.resize(DLNJOINTS);
		upperLeft.set(0, 0);
		lowerRight.set(0, 0);
	}
};

class controlApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void parseLatestPose();

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
	bool shouldReadPose;
	int timeLastSent;
	int timeoutMillis = 3000 ;
	int timeLatestPose;

	ofBuffer recv_buf;
	ofxJSONElement json;

	vector<dlBody> currentBodies;
	int currentNBody;

	ofVec2f sceneUpperLeft, sceneLowerRight;
};


