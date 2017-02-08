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
	ofVec4f calcCrop();
	ofVec4f keepViewPort(ofVec4f centerSize);

	vector<ofxDatGuiComponent*> components;
	ofxDatGuiTextInput* status;
	ofxDatGuiSlider* stabilizeSlider;

	void onResetButtonEvent(ofxDatGuiButtonEvent e);
	void onCropToBoundsButtonEvent(ofxDatGuiButtonEvent e);

	void onZoomSliderEvent(ofxDatGuiSliderEvent e);
	void onStabilizeSliderEvent(ofxDatGuiSliderEvent e);

	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onMatrixEvent(ofxDatGuiMatrixEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);

	float cropWidth, cropUpperLeftX, cropUpperLeftY, zoom1Max, stabilize;
	ofVec4f lastCenterSize;

	shared_ptr<rtPoseApp> rtPose;
	bool drawBounds;

};

