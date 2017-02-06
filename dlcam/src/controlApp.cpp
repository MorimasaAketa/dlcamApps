/*
* GuiApp.cpp
*
*  Created on: Oct 28, 2014
*      Author: arturo
*/

#include "controlApp.h"

void controlApp::setup() {
	ofBackground(0);
	ofSetVerticalSync(false);
	isFrameNew = false;
	shouldWrite = false;
	fbo.allocate(1280, 720, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 255, 255);
	fbo.end();

}

void controlApp::update() {

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

}

void controlApp::draw() {
	//tex2160.draw(0, 0, 1280, 720);
	if (shouldWrite) {
		fbo.draw(0, 0, 1280, 720);
		ofImage img;
		img.grabScreen(0, 0, 1280, 720);
		turbo.save(&img, "turbograbbed.jpg", 80);
		shouldWrite = false;
		img.clear();
	}

}
