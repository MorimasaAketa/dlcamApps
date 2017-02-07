#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup() {

	auto deviceList = ofxBlackmagic::Iterator::getDeviceList();

	for (auto device : deviceList) {
		auto input = shared_ptr<ofxBlackmagic::Input>(new ofxBlackmagic::Input());

		static int index = 0;
		auto mode = index++ == 0 ? bmdModeHD1080p25 : bmdModeHD1080p24;
		input->startCapture(device, mode);
		input->setUseTexture(true);

		this->inputs.push_back(input);
	}
	ofBackground(0);

	tex2160.allocate(3840, 2160, GL_RGBA);
	control->tex2160 = tex2160;


	fbo2160.allocate(3840, 2160, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 255, 255);
	fbo2160.end();

	control->mainFbo = fbo2160;
	cropCenter.set(1920, 1080);
	cropSize.set(3840, 2160);
	drawBounds = false;
}

//--------------------------------------------------------------
void ofApp::update() {
	for (auto input : this->inputs) {
		input->update();
	}

	ofImage imgbuf, img720;
	ofBuffer jpegbuf;

	for (auto input : this->inputs) {
		if (input->isFrameNew()) {
			control->isFrameNew = true;
			tex2160.clear();
			tex2160 = input->getTexture(); // faster?
			fbo2160.begin();
			tex2160.draw(0, 0, 3840, 2160);
			fbo2160.end();
		}
	}



}

//--------------------------------------------------------------
void ofApp::draw() {
	float x = 0; float y = 0;

	//	float width = 1920 / 2; float height = 1080 / 2;
	float width = 3840; float height = 2160;

	for (auto input : this->inputs) {
		//fbo2160.draw(0, 0, 1920,1080);

		//fbo2160.draw(upperLeft.x * zoomRatio, upperLeft.y * zoomRatio,
		//	3840 * zoomRatio, 2160 * zoomRatio);
		//fbo2160.draw(-1920 / 2, -1080 / 2, 3840, 2160);
		//fbo2160.draw(upperLeft.x * zoomRatio, upperLeft.y * zoomRatio,
		//	3840 * zoomRatio, 2160 * zoomRatio);


		ofVec2f cropUpperLeft  = cropCenter - cropSize/2;
		ofVec2f upperLeft = - cropUpperLeft; // 4K geometory;
		float offsetx = upperLeft.x / (3840 / cropSize.x);
		float offsety = upperLeft.y / (2160 / cropSize.y);
		fbo2160.draw(
			offsetx, offsety,
			1920 * (3840 / cropSize.x), 
			1080 * (2160 / cropSize.y)
		);
		//ofLogNotice() << offsetx;
	}
	if (this->inputs.empty()) {
		ofDrawBitmapString("No BlackMagic input devices found", 20, 20);
	}

	if (drawBounds && control->currentNBody) {
		ofPushMatrix();
		ofSetColor(ofColor::yellow);
		ofNoFill();
		ofSetLineWidth(3);
		ofScale(1920.0 / 1280.0, 1080.0 / 720.0, 1.0);
		ofDrawRectangle(control->sceneUpperLeft.x, control->sceneUpperLeft.y,
			control->sceneLowerRight.x - control->sceneUpperLeft.x,
			control->sceneLowerRight.y - control->sceneUpperLeft.y);
		ofSetColor(255);
		ofPopMatrix();
	}


}
//--------------------------------------------------------------
bool ofApp::checkCropConstraint(ofVec2f center, ofVec2f size) {
	ofVec2f cropUpperLeft = center - size / 2;
	ofVec2f cropLowerRight = center + size / 2;
	bool check = true;
	ofLog() << size.x;
	if (cropUpperLeft.x < 0 || cropUpperLeft.y < 0
		|| cropLowerRight.x > 3840 || cropLowerRight.y > 2160
		|| size.x > 3840 || size.y > 2160 
		|| size.x < 1920 || size.y < 1080 ) {
		check = false;
	}

	return check;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofVec2f newCropSize;
	bool zoomPressed = false;
	if (key == 'z') {
		newCropSize = cropSize * 0.999;
		zoomPressed = true;
	}
	if (key == 'a') {
		newCropSize = cropSize * 1.001;
		zoomPressed = true;
	}

	if(zoomPressed && checkCropConstraint(cropCenter, newCropSize)) {
		cropSize = newCropSize;
		ofLogNotice() << "zooming";
	}

	// cropUpperLeft  = cropCenter - cropsize/2;
	// croplowerright = cropcenter + cropsize/2;
	// cropUpperleft > 0,0
	// cropLowerRight >3820, 2160
	// crop size < 3830, 2160
	// crop size > 1920, 1080

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'b') {
		drawBounds = !drawBounds;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------

void ofApp::mouseDragged(int x, int y, int button){
	/*
	if (button == OF_MOUSE_BUTTON_LEFT) {
		upperLeft.x += (x - positionMousePressed.x) * zoomRatio;
		upperLeft.y += (y - positionMousePressed.y) * zoomRatio;
		if (upperLeft.x * zoomRatio < -1920) {
			upperLeft.x = -1920;
		}else if (upperLeft.x * zoomRatio > 0) {
			upperLeft.x = 0;
		}

		if (upperLeft.y * zoomRatio < -1080) {
			upperLeft.y = -1080;
		}
		else if (upperLeft.y * zoomRatio > 0) {
			upperLeft.y = 0;
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (button == OF_MOUSE_BUTTON_LEFT) {
		positionMousePressed.set(x, y);
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
