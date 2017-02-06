#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	auto deviceList = ofxBlackmagic::Iterator::getDeviceList();

	for(auto device : deviceList) {
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
}

//--------------------------------------------------------------
void ofApp::update(){
	for(auto input : this->inputs) {
		input->update();
	}

	ofImage imgbuf, img720;
	ofBuffer jpegbuf;

	for (auto input : this->inputs) {
		if(input->isFrameNew()){
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
void ofApp::draw(){
	float x = 0; float y = 0;

//	float width = 1920 / 2; float height = 1080 / 2;
	float width = 3840; float height = 2160;

	for (auto input : this->inputs) {
		//fbo2160.draw(0, 0, 1920, 1080);
		fbo2160.draw(-1920, -1080, 3840, 2160);
	}
	if (this->inputs.empty()) {
		ofDrawBitmapString("No BlackMagic input devices found", 20, 20);
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
