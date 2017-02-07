#pragma once
#include "ofxBlackmagic.h"
#include "ofMain.h"
#include "controlApp.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		bool checkCropConstraint(ofVec2f center, ofVec2f size);

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
		ofTexture tex2160;
		ofFbo fbo, fbo2160;

		ofVec2f cropCenter;
		ofVec2f cropSize;

		// cropUpperLeft  = cropCenter - cropsize/2;
		// upperLeft = - cropUperLeft
		
		// cropUpperLeft  = cropCenter - cropsize/2;
		// croplowerright = cropcenter + cropsize/2;
		// cropUpperleft > 0,0
		// cropLowerRight >3820, 2160
		// crop size < 3830, 2160
		// crop size > 1920, 1080

		vector<vector<ofVec3f>> currentBodies;
	

		ofVec2f positionMousePressed;
	

		shared_ptr<controlApp> control;

		bool drawBounds;
};
