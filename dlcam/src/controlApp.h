#pragma once

#define HOST "localhost" // 受信側のIPアドレス
#define PORT 8000 // 受信側のポート番号

#include "ofMain.h"
#include "ofxTurboJpeg.h"

class controlApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofFbo fbo, mainFbo;
	ofxTurboJpeg turbo;
	ofPixels pix720;
	ofTexture tex720, tex2160;
	int64_t img_count;
	bool isFrameNew;
	bool shouldWrite;
};


