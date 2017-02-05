#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	auto deviceList = ofxBlackmagic::Iterator::getDeviceList();

	for(auto device : deviceList) {
		auto input = shared_ptr<ofxBlackmagic::Input>(new ofxBlackmagic::Input());

		static int index = 0;
		auto mode = index++ == 0 ? bmdModeHD1080p25 : bmdModeHD1080p24;
		input->startCapture(device, mode);
		this->inputs.push_back(input);
	}

	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
	for(auto input : this->inputs) {
		input->update();
	}

	ofImage imgbuf;
	ofBuffer jpegbuf;

	tex720.clear();
	tex720.allocate(1280, 720, GL_RGB);
	for (auto input : this->inputs) {
		if(input->isFrameNew()){
			pix720 = input->getPixels();
			pix720.resize(1280, 720);
			imgbuf.setFromPixels(pix720);
			turbo.compress(imgbuf, 75, jpegbuf);

			//ofLogNotice() << "img size:" << jpegbuf.size();
			tex720.loadData(pix720);
		}
	}



}

//--------------------------------------------------------------
void ofApp::draw(){
	float x = 0; float y = 0;

//	float width = 1920 / 2; float height = 1080 / 2;
	float width = 3840; float height = 2160;

	for (auto input : this->inputs) {
		tex720.draw(0, 0);
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
