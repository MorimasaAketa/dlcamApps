#include "ofApp.h"
#include "ofMain.h"
#include "controlApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;


	settings.width = 1280;
	settings.height = 720;
	settings.setPosition(ofVec2f(0, 0));
	settings.resizable = false;
	shared_ptr<ofAppBaseWindow> controlWindow = ofCreateWindow(settings);

	settings.width = 1920;
	settings.height = 1080;
	settings.setPosition(ofVec2f(300, 0));
	settings.resizable = false;
	settings.shareContextWith = controlWindow;

	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);


	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<controlApp> controlApp(new controlApp);
	mainApp->control = controlApp;

	ofRunApp(controlWindow, controlApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
