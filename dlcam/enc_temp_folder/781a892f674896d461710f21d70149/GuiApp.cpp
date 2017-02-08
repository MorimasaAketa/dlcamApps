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
	component = new ofxDatGuiButton("RESET");
	component->setPosition(x, y);
	component->onButtonEvent(this, &GuiApp::onResetButtonEvent);
	components.push_back(component);
	
	y += component->getHeight() + p;
	component = new ofxDatGuiButton("CROP TO BOUNDS");
	component->setPosition(x, y);
	component->onButtonEvent(this, &GuiApp::onCropToBoundsButtonEvent);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiSlider("cropWidth", 1920, 3840, 3840);
	component->setPosition(x, y);
	component->onSliderEvent(this, &GuiApp::onCropWidthEvent);
	components.push_back(component);

	parameters.setName("parameters");
	parameters.add(guiCropWidth.set("cropWidth",3840,1920,3840));
	parameters.add(guiCropUpperLeftX.set("cropUpperLeftX", 0, 0, 960));
	parameters.add(guiCropUpperLeftY.set("cropUpperLeftY", 0, 0, 540));
	gui.setup(parameters);
	ofBackground(0);
	ofSetVerticalSync(false);
	drawBounds = false;
	cropSliderValid = true;

	cropWidth = 3840;
	cropUpperLeftX = 0;
	cropUpperLeftY = 0;
	
}

void GuiApp::update(){
	for (int i = 0; i<components.size(); i++) components[i]->update();
	cropToBounds();
	//cout << "cropWidth: " << cropWidth << endl;
/*	if (cropSliderValid) {
		cropWidth = guiCropWidth.get();
		cropUpperLeftX = guiCropUpperLeftX.get();
		cropUpperLeftY = guiCropUpperLeftY.get();
	}
*/

	if (rtPose->timeLatestPose) {
		string s("GPGPU is DETECTING ");
		s += ofToString(rtPose->currentNBody);
		s += " HUMAN";
		status->setText(s);
	}
}



void GuiApp::draw(){
	//gui.draw();
	for (int i = 0; i<components.size(); i++) components[i]->draw();

}


void GuiApp::keyPressed(int key) {
	if (key == 'b') {
		drawBounds = !drawBounds;
	}

}

ofVec3f GuiApp::calcCrop() {
	ofVec3f crop(0,0,3480); // left, top, width
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

		crop.set(center.x - (size.x / 2.0), center.y - (size.y / 2.0), size.x);
		/*
		cropWidth = size.x;
		cropUpperLeftX = center.x - (size.x / 2.0);
		cropUpperLeftY = center.y - (size.y / 2.0);
	
		cout << "size: " << size.x << ", " << size.y
		<< " center: " << center.x << ", " << center.y << endl;
		cout << "ulX: " << cropUpperLeftX << ", " << cropUpperLeftY << endl;
		*/
	}
	return crop;
}

void GuiApp::cropToBounds() {
	ofVec3f crop = calcCrop();
	cropWidth = crop.z;
	cropUpperLeftX = crop.x;
	cropUpperLeftY = crop.y;
}


/*
event listeners
*/

void GuiApp::onResetButtonEvent(ofxDatGuiButtonEvent e)
{
	cropWidth = 3840;
	cropUpperLeftX = 0.0;
	cropUpperLeftY = 0.0;
	cout << "onButtonEvent: " << e.target->getLabel() << endl;

}

void GuiApp::onCropToBoundsButtonEvent(ofxDatGuiButtonEvent e)
{
	cropToBounds();
}

void GuiApp::onCropWidthEvent(ofxDatGuiSliderEvent e)
{
	cropWidth = (float)e.value;

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
