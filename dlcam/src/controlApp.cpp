/*
* GuiApp.cpp
*
*  Created on: Oct 28, 2014
*      Author: arturo
*/


#include "controlApp.h"

#include "ofxZmq.h"


void controlApp::setup() {
	ofBackground(0);
	ofSetVerticalSync(false);
	isFrameNew = false;
	shouldWrite = false;
	fbo.allocate(1280, 720, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 255, 255);
	fbo.end();

	// start server
	publisher = new ofxZmqPublisher();
	publisher->setHighWaterMark(1);
	publisher->bind("tcp://10.0.0.24:8888");

	// start client
	subscriber = new ofxZmqSubscriber();
	subscriber->setHighWaterMark(1);
	subscriber->connect("tcp://10.0.0.20:7777");
	ackWaiting = false;

}

void controlApp::update() {
	while (subscriber->hasWaitingMessage())
	{
		ofBuffer data;
		subscriber->getNextMessage(data);
		ackWaiting = false
			;
		cout << "received data: " << data << endl;
	}

	if(isFrameNew){
		fbo.begin();
		ofClear(0, 255);
		ofSetColor(255);
		mainFbo.draw(0, 0, 1280, 720);
		fbo.end();
		isFrameNew = false;
		shouldWrite = true;
	}

	img_count++;
	if (ackWaiting && timeLastSent + timeoutMillis < ofGetElapsedTimeMillis()) {
		timeLastSent = 0;
		ackWaiting = false;
	}

}

void controlApp::draw() {
	//tex2160.draw(0, 0, 1280, 720);
	fbo.draw(0, 0, 1280, 720);
	if (shouldWrite && !ackWaiting) {
		ofImage img;
		img.grabScreen(0, 0, 1280, 720);
		turbo.save(&img, "turbograbbed.jpg", 80);

		turbo.save(send_buf, img.getPixelsRef(), 90);
		//send_bu = ofBufferFromFile("turbograbbed.jpg",true);
		publisher->send(send_buf);
		timeLastSent = ofGetElapsedTimeMillis();
		ofLog() << "send";
		ackWaiting = true;
		shouldWrite = false;
		img.clear();
	}

}

void controlApp::keyPressed(int key) {

}
