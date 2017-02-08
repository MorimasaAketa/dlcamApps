#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"
#include "rtPoseApp.h"

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void cropToBounds();
	ofVec3f calcCrop();
	
	vector<ofxDatGuiComponent*> components;
	ofxDatGuiTextInput* status;

	void onResetButtonEvent(ofxDatGuiButtonEvent e);
	void onCropToBoundsButtonEvent(ofxDatGuiButtonEvent e);

	void onCropWidthEvent(ofxDatGuiSliderEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onMatrixEvent(ofxDatGuiMatrixEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);



	ofParameterGroup parameters;
	ofParameter<float> guiCropWidth;
	ofParameter<float> guiCropUpperLeftX;
	ofParameter<float> guiCropUpperLeftY;
	ofParameter<ofColor> color;
	ofxPanel gui;

	float cropWidth, cropUpperLeftX, cropUpperLeftY;

	shared_ptr<rtPoseApp> rtPose;
	bool drawBounds;
	bool cropSliderValid;

};

