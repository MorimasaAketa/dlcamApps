#include "ofApp.h"

const int WIDTH = 1280;
const int HEIGHT = 720;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);

	if (device_name == "dummy")
	{
		video_stream = std::make_unique<DummyVideoStream>(WIDTH, HEIGHT);
	}
	else
	{
		video_stream = std::make_unique<BlackmagicVideoStream>(device_name);
	}

	//pub.bind(port_name);

	fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 255, 255);
	fbo.end();

	if (debug)
	{
		font.load("verdana.ttf", 32);
	}
}

//--------------------------------------------------------------
void ofApp::exit()
{
	video_stream.reset();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (video_stream)
	{
		video_stream->update();

		if (video_stream->isFrameNew())
		{
			fbo.begin();
			ofClear(0, 255);

			ofSetColor(255);

			image.setFromPixels(video_stream->getPixels());
			image.draw(0, 0, WIDTH, HEIGHT);

			if (debug)
			{
				std::stringstream ss;
				ss << "time: " << ofToString(ofGetElapsedTimef(), 1) << endl;
				ss << "device: " << device_name << endl;
				ss << "port: " << port_name << endl;

				ofPushStyle();
				font.drawString(ss.str(), 32, 64);
				ofPopStyle();
			}

			fbo.end();

			fbo.readToPixels(pix);

			TurboJpegIO::Arguments args;
			args.width = pix.getWidth();
			args.height = pix.getHeight();
			args.pixel_format = TurboJpegIO::RGBX;
			int N = pix.getTotalBytes();

			jpeg.write(args, pix.getData(), N, [this](const uint8_t* data, size_t size) {
				writeFile(data, size);
			}, 50);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

void ofApp::writeFile(const uint8_t *buf, size_t size) {
	return;
	ofLogNotice() << "write file to";
	ofBuffer b;
	b.allocate(size);
	b.set((const char *)buf, size);
	ofFile newFile(ofToDataPath("turbo.jpg"), ofFile::WriteOnly);
	newFile.create();
	newFile.writeFromBuffer(b);
	newFile.close();

}
//////////////////////////////////////////////////////////////////////////

DummyVideoStream::DummyVideoStream(int width, int height)
{
	pix.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
}

void DummyVideoStream::update()
{
	pix.setColor(ofColor(ofRandom(255), ofRandom(255), ofRandom(255)));
}

//////////////////////////////////////////////////////////////////////////

BlackmagicVideoStream::BlackmagicVideoStream(std::string device_name)
{
	{
		auto deviceList = ofxBlackmagic::Iterator::getDeviceList();
		for (auto device : deviceList)
		{
#ifdef TARGET_WIN32
			BSTR displayName;
#elif defined(TARGET_OSX)
			CFStringRef displayName;
#endif

			device.device->GetDisplayName(&displayName);
			std::string name = ofxBlackmagic::Utils::toString(displayName);

			if (name == device_name) {
				cout << name << endl;
				auto mode = bmdModeHD1080p30;
				input = shared_ptr<ofxBlackmagic::Input>(new ofxBlackmagic::Input());
				input->startCapture(device, mode);
			}
		}
	}

	if (!input)
	{
		std::cerr << "no device found" << std::endl;
		ofExit(-1);
	}
}


BlackmagicVideoStream::~BlackmagicVideoStream()
{
	input.reset();
}

void BlackmagicVideoStream::update()
{
	input->update();

	pix = input->getPixels();
	pix.resize(WIDTH, HEIGHT);
}

bool BlackmagicVideoStream::isFrameNew()
{
	return input->isFrameNew();
}
