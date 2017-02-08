/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

void GuiApp::setup(){
	int x = 50;
	int y = 50;
	int p = 40;
	ofSetWindowPosition(0, 0);
	ofxDatGuiComponent* component;

	status = new ofxDatGuiTextInput("status", "waiting for detection...");
	status->setPosition(x, y);
	//component->onTextInputEvent(this, &ofApp::onTextInputEvent);
	components.push_back(status);

	y += status->getHeight() + p;
	component = new ofxDatGuiSlider("ZOOM", 0, 100, 0);
	component->setPosition(x, y);
	component->onSliderEvent(this, &GuiApp::onZoomSliderEvent);
	components.push_back(component);

	stabilizeSlider = new ofxDatGuiSlider("STABILIZE", 0, 100, 0);
	y += stabilizeSlider->getHeight() + p;
	stabilizeSlider->setPosition(x, y);
	stabilizeSlider->onSliderEvent(this, &GuiApp::onStabilizeSliderEvent);
	components.push_back(stabilizeSlider);

	y += component->getHeight() + p;
	component = new ofxDatGuiButton("RESET ZOOM");
	component->setPosition(x, y);
	component->onButtonEvent(this, &GuiApp::onResetButtonEvent);
	components.push_back(component);
	
	y += component->getHeight() + p;
	component = new ofxDatGuiButton("CROP TO BOUNDS");
	component->setPosition(x, y);
	component->onButtonEvent(this, &GuiApp::onCropToBoundsButtonEvent);
	components.push_back(component);

	
	components.push_back(component);

	ofBackground(0);
	ofSetVerticalSync(false);
	drawBounds = false;

	cropWidth = 3840;
	cropUpperLeftX = 0;
	cropUpperLeftY = 0;
	zoom1Max = 0.0;
	stabilize = 0.0;
	lastCenterSize.set(1920, 1080, 3840, 2160);
	
}

void GuiApp::update(){
	for (int i = 0; i<components.size(); i++) components[i]->update();
//	cropToBounds();

	if (rtPose->timeLatestPose) {
		string s("GPGPU is DETECTING ");
		s += ofToString(rtPose->currentNBody);
		s += " HUMAN";
		status->setText(s);
	}
	if (zoom1Max > 0.0) {
		ofVec4f centerSize = calcCrop();
		centerSize.z = ofMap(zoom1Max, 0.0, 1.0, centerSize.z, 3840.0); // width
		centerSize.w = 2160 * centerSize.z / 3840; // height;

		ofVec4f keep = keepViewPort(centerSize);
		
		// stabilize
		ofVec2f newCenter(keep.x, keep.y);
		ofVec2f lastCenter(lastCenterSize.x, lastCenterSize.y);
		if (newCenter.distance(lastCenter) > stabilize) {
			cropWidth = keep.z;
			cropUpperLeftX = keep.x - keep.z / 2.0;
			cropUpperLeftY = keep.y - keep.w / 2.0;
			lastCenterSize = keep;
		}
		
	}
}



void GuiApp::draw(){
	for (int i = 0; i<components.size(); i++) components[i]->draw();

}


void GuiApp::keyPressed(int key) {
	if (key == 'b') {
		drawBounds = !drawBounds;
	}

}

ofVec4f GuiApp::calcCrop() {
	ofVec4f centerSize(1920, 1080, 3480,2160); // centerX, centerY, width, height;
	if (rtPose->currentNBody > 0) {
		// here, 1280x720;
		ofVec2f center = (rtPose->sceneUpperLeft + rtPose->sceneLowerRight) / 2.0;
		ofVec2f size = rtPose->sceneLowerRight - rtPose->sceneUpperLeft;
		size.y *= 1.4;
		if (size.y > 2160.0) {
			size.y = 2160.0;
		}
		float ratio = size.x / size.y;
		if (ratio > 16.0 / 9.0) { // landscape; keep width, increase height;
			size.y = size.x * (9.0 / 16.0);
		}
		else { // portrait; keep height, increase width;
			size.x = size.y * (16.0 / 9.0);
		}

		center = center * 3840.0 / 1280.0;
		size = size * 3840.0 / 1280.0;
		// keep in original
		float left = center.x - size.x / 2.0;
		float top = center.y - size.y / 2.0;
		float right = center.x + size.x / 2.0;
		float bottom = center.y + size.y / 2.0;

		if (left < 0.0) {
			center.x += left * -1.0;
		}
		else if (right > 3840.0) {
			center.x -= right - 3840.0;
		}

		if (top < 0.0) {
			center.y += top * -1.0;
		}
		else if (bottom > 2160.0) {
			center.y -= bottom - 2160.0;
		}
		centerSize.set(center.x, center.y, size.x, size.y);
//		crop.set(center.x - (size.x / 2.0), center.y - (size.y / 2.0), size.x);
		
	}
	return centerSize;
}

ofVec4f GuiApp::keepViewPort(ofVec4f centerSize) {
	float left = centerSize.x - centerSize.z / 2.0;
	float top = centerSize.y - centerSize.w / 2.0;
	float right = centerSize.x + centerSize.z / 2.0;
	float bottom = centerSize.y + centerSize.w / 2.0;

	if (left < 0.0) {
		centerSize.x += left * -1.0;
	}
	else if (right > 3840.0) {
		centerSize.x -= right - 3840.0;
	}

	if (top < 0.0) {
		centerSize.y += top * -1.0;
	}
	else if (bottom > 2160.0) {
		centerSize.y -= bottom - 2160.0;
	}

	ofVec4f newCenterSize(centerSize.x, centerSize.y, centerSize.z, centerSize.w);

	return newCenterSize;

}
void GuiApp::cropToBounds() {
	ofVec4f centerSize = calcCrop();
	cropWidth = centerSize.z;
	cropUpperLeftX = centerSize.x - centerSize.z / 2.0;
	cropUpperLeftY = centerSize.y - centerSize.w / 2.0;
}


/*
event listeners
*/

void GuiApp::onResetButtonEvent(ofxDatGuiButtonEvent e)
{
	zoom1Max = 0.0;
	cropWidth = 3840;
	cropUpperLeftX = 0.0;
	cropUpperLeftY = 0.0;
	cout << "onButtonEvent: " << e.target->getLabel() << endl;

}

void GuiApp::onCropToBoundsButtonEvent(ofxDatGuiButtonEvent e)
{
	cropToBounds();
}

void GuiApp::onZoomSliderEvent(ofxDatGuiSliderEvent e)
{
	float zoom = (float)e.value;
	zoom1Max = (100 - zoom) / 100;

	
	cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
}

void GuiApp::onStabilizeSliderEvent(ofxDatGuiSliderEvent e)
{
	stabilize = (float)e.value;
	//zoom1Max = (100 - zoom) / 100;
	cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
}

void GuiApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	cout << "onToggleEvent: " << e.target->getLabel() << "::" << e.target->getChecked() << endl;
}

void GuiApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
	cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
}

void GuiApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
	cout << "onDropdownEvent: " << e.child << endl;
}

void GuiApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
	cout << "onMatrixEvent: " << e.child << "::" << e.enabled << endl;
}

void GuiApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
	cout << "onColorPickerEvent: " << e.color << endl;
}

void GuiApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
	cout << "on2dPadEvent: " << e.x << "::" << e.y << endl;
}

void GuiApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	cout << "onButtonEvent: " << e.text << endl;
}
