#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"
#include "rtPoseApp.h"

enum DLShot { DLFullFigure, DLKneeShot, DLWeistShot, DLBustShot};
enum DLPanFrom {DLPanNot, DLPanFromLeft, DLPanFromRight};

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void cropToBounds();
	void updateCrop(ofVec4f centerSize);
	ofVec4f calcCrop();
	ofVec4f keepViewPort(ofVec4f centerSize);

	vector<ofxDatGuiComponent*> components;
	ofxDatGuiTextInput* status;
	ofxDatGuiSlider* zoomSlider;
	ofxDatGuiSlider* stabilizeSlider;
	ofxDatGuiSlider* autoZoomSpeedSlider;
	ofxDatGuiToggle* autoZoomInToggle;
	ofxDatGuiToggle* autoZoomOutToggle;
	ofxDatGuiToggle* zoomToFullFigure;
	ofxDatGuiToggle* zoomToKneeShot;
	ofxDatGuiToggle* zoomToWeistShot;
	ofxDatGuiToggle* zoomToBustShot;


	void onZoomSliderEvent(ofxDatGuiSliderEvent e);
	void onStabilizeSliderEvent(ofxDatGuiSliderEvent e);
	void onResetButtonEvent(ofxDatGuiButtonEvent e);
	void onCropToBoundsButtonEvent(ofxDatGuiButtonEvent e);
	void onAutoZoomSpeedSliderEvent(ofxDatGuiSliderEvent e);
	void onAutoZoomInToggleEvent(ofxDatGuiToggleEvent e);
	void onAutoZoomOutToggleEvent(ofxDatGuiToggleEvent e);
	void onShowBoundsToggleEvent(ofxDatGuiToggleEvent e);
	void onZoomToFullFigureToggleEvent(ofxDatGuiToggleEvent e);
	void onZoomToKneeShotToggleEvent(ofxDatGuiToggleEvent e);
	void onZoomToWeistShotToggleEvent(ofxDatGuiToggleEvent e);
	void onZoomToBustShotToggleEvent(ofxDatGuiToggleEvent e);
	//void onPanFromLeftButtonEvent(ofxDatGuiButtonEvent e);
	//void onPanFromRightButtonEvent(ofxDatGuiButtonEvent e);


	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onMatrixEvent(ofxDatGuiMatrixEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);

	float cropWidth, cropUpperLeftX, cropUpperLeftY, zoom1Max, stabilize;
	float autoZoomSpeed;
	bool drawBounds, autoZoomIn, autoZoomOut;
	int shot;
	int pan;
	

	ofVec4f lastCenterSize, autoZoomCenterSize;

	shared_ptr<rtPoseApp> rtPose;

};

