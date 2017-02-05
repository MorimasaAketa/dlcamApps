#pragma once


#define HOST "localhost" // 受信側のIPアドレス
#define PORT 8000 // 受信側のポート番号


#include "ofxBlackmagic.h"
#include "ofMain.h"
#include "ofxTurboJpeg.h"
#include "ofxOsc.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		vector<shared_ptr<ofxBlackmagic::Input> > inputs;
		ofxTurboJpeg turbo;
		ofPixels pix720;
		ofTexture tex720, tex2160;
		ofFbo fbo;
		ofxOscSender sender;
		int64_t img_count;
};
