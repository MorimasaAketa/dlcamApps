/*
* rtPoseApp.cpp
*
*  2017-02-07 
*      Author: Morimasa Aketa
*/


#include "rtPoseApp.h"

#include "ofxZmq.h"


void rtPoseApp::setup() {
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
	shouldReadPose = false;

	currentBodies.resize(0);
	timeLatestPose = false;
	detectionOn = true;
}

void rtPoseApp::update() {
	while (subscriber->hasWaitingMessage())
	{
		recv_buf.clear();
		subscriber->getNextMessage(recv_buf);
		ackWaiting = false;
	//	cout << "received data: " << recv_buf << endl;
		shouldReadPose = true;
		timeLatestPose = ofGetElapsedTimeMillis();
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

	if (shouldReadPose) {
		parseLatestPose();
	}

}

void rtPoseApp::draw() {
	//tex2160.draw(0, 0, 1280, 720);
	fbo.draw(0, 0, 1280, 720);
	if (detectionOn && shouldWrite && !ackWaiting) {
		ofImage img;
		img.grabScreen(0, 0, 1280, 720);
		turbo.save(&img, "turbograbbed.jpg", 80);

		turbo.save(send_buf, img.getPixelsRef(), 90);
		//send_bu = ofBufferFromFile("turbograbbed.jpg",true);
		publisher->send(send_buf);
		timeLastSent = ofGetElapsedTimeMillis();
		ackWaiting = true;
		shouldWrite = false;
		img.clear();
	}

}

void rtPoseApp::keyPressed(int key) {

}

void rtPoseApp::parseLatestPose() {
	currentNBody = 0;
	bool res = json.parse(recv_buf.getText());

	if (res) {
		//ofLogNotice() << "parse Success";
	}
	else {
		//ofLogNotice() << "parse failed";
		return;
	}
	currentBodies.resize(0);

	vector<ofVec3f> joints;
	ofVec2f min(0,0), max(0,0);



	sceneUpperLeft = min;
	sceneLowerRight = min;
	for (Json::ArrayIndex i = 0; i < json["bodies"].size(); i++ ) {
		min.set(0,0);
		max.set(0, 0);
		

		joints.resize(0);
		for (int j = 0; j < DLNJOINTS*3; j += 3) {
			ofVec3f joint;
			joint.set(
				json["bodies"][i]["joints"][j].asFloat(),
				json["bodies"][i]["joints"][j + 1].asFloat(),
				json["bodies"][i]["joints"][j + 2].asFloat()
			);
			joints.push_back(joint);
			if (joint.x > max.x) {
				max.x = joint.x;
			}
			if (joint.y > max.y) {
				max.y = joint.y;
			}
		}
		min.x = max.x;		
		min.y = max.y;

		for (int j = 0; j < DLNJOINTS; j++) {
			if ( joints[j] == ofVec3f::zero() ) {
				continue;
			}
			if (joints[j].x < min.x) {
				min.x = joints[j].x;
			}
			if (joints[j].y < min.y) {
				min.y = joints[j].y;
			}
		}

		dlBody body;
		body.setup();
		copy(joints.begin(), joints.end(), body.joints.begin());
		body.upperLeft = min;
		body.lowerRight = max;
		currentBodies.push_back(body);

		if (sceneLowerRight.x < max.x) {
			sceneLowerRight.x = max.x;
		}
		if (sceneLowerRight.y < max.y) {
			sceneLowerRight.y = max.y;
		}
	}
	sceneUpperLeft = sceneLowerRight;
	for (int i = 0; i < currentBodies.size(); i++) {
		if (currentBodies[i].upperLeft.x < sceneUpperLeft.x) {
			sceneUpperLeft.x = currentBodies[i].upperLeft.x;
		}
		if (currentBodies[i].upperLeft.y < sceneUpperLeft.y) {
			sceneUpperLeft.y = currentBodies[i].upperLeft.y;
		}
	}
	shouldReadPose = false;
	currentNBody = currentBodies.size();
	/*
	ofLogNotice() << currentNBody << " bodies; "
		<< sceneUpperLeft.x << ", " << sceneUpperLeft.y << " - "
		<< sceneLowerRight.x << ", " << sceneLowerRight.y;
		*/

}

void rtPoseApp::stopDetection() {
	detectionOn = false;
	currentNBody = 0;
}

void rtPoseApp::startDetection() {
	detectionOn = true;
	currentNBody = 0;
}