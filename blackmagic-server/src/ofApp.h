#pragma once

#include "ofMain.h"

#define __STRUCT__ struct
#define interface __STRUCT__
#include "ofxBlackMagic.h"

#include "TurboJpegIO/TurboJpegIO.h"

class VideoStream
{
public:

	virtual ~VideoStream() {}

	virtual void update() {}
	virtual bool isFrameNew() { return false; }
	virtual const ofPixels& getPixels() const = 0;
};

class DummyVideoStream : public VideoStream
{
public:

	DummyVideoStream(int width, int height);

	void update() override;
	bool isFrameNew() override { return true; }
	const ofPixels& getPixels() const override { return pix; }

protected:

	ofPixels pix;
};

class BlackmagicVideoStream : public VideoStream
{
public:

	BlackmagicVideoStream(std::string device_name);
	~BlackmagicVideoStream();

	void update() override;
	bool isFrameNew() override;
	const ofPixels& getPixels() const override { return pix; }

protected:

	std::shared_ptr<ofxBlackmagic::Input> input;
	ofPixels pix;
};

class ofApp : public ofBaseApp {
public:

	
	ofPixels pix;
	TurboJpegIO::Writer jpeg;
	//NanomsgIO::Publisher pub;

	std::string device_name;
	std::string port_name;

	std::unique_ptr<VideoStream> video_stream;

	ofFbo fbo;
	ofImage image;

	bool show_window = true;
	bool debug = false;

	ofTrueTypeFont font;

	void setup();
	void exit();

	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void writeFile(const uint8_t *buf, size_t size);
		
};
