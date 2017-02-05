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
	sender.setup(HOST, PORT);
	img_count = 0;
	ofBackground(0);


	fbo.allocate(1280, 720, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 255, 255);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
	for(auto input : this->inputs) {
		input->update();
	}

	ofImage imgbuf, img720;
	ofBuffer jpegbuf;

	tex720.clear();
	tex720.allocate(1280, 720, GL_RGB);
	for (auto input : this->inputs) {
		if(input->isFrameNew()){
	
			fbo.begin();
			ofClear(0, 255);

			ofSetColor(255);

			//imgbuf.setFromPixels(input->getPixels());
			tex2160 = input->getTexture(); // faster?
			//imgbuf.draw(0, 0, 1280, 720);
			tex2160.draw(0, 0, 1280, 720);
			fbo.end();
			fbo.readToPixels(pix720);
			img720.setFromPixels(pix720);

			//turbo.save(jpegbuf, pix720, 90);

			//turbo.save(&img720, "turbo.jpg", 75);

			// bool fileWritten = ofBufferToFile("img.jpg", jpegbuf);
			img_count++;
			ofxOscMessage m;
			m.setAddress("/pose/jpeg");
			m.addInt64Arg(img_count);
//			m.addBlobArg(jpegbuf);
//			sender.sendMessage(m);
			//ofLogNotice() << "img size:" << &jpegbuf.size();
//			ofLogNotice() << "img count:" << img_count;
		}
	}



}

//--------------------------------------------------------------
void ofApp::draw(){
	float x = 0; float y = 0;

//	float width = 1920 / 2; float height = 1080 / 2;
	float width = 3840; float height = 2160;

	for (auto input : this->inputs) {
		fbo.draw(0, 0, 1280, 720);
	}
	if (this->inputs.empty()) {
		ofDrawBitmapString("No BlackMagic input devices found", 20, 20);
	}
	ofImage img;
	img.grabScreen(0, 0, 1280, 720);
	//turbo.save(&img, "turbograbbed.jpg", 80);
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
