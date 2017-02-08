#include "ofApp.h"
#include "ofMain.h"
#include "rtPoseApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;

	

	settings.width = 1280;
	settings.height = 720;
	settings.setPosition(ofVec2f(0, 0));
	settings.resizable = false;
	shared_ptr<ofAppBaseWindow> rtPoseWindow = ofCreateWindow(settings);

	settings.width = 640;
	settings.height = 720;
	settings.setPosition(ofVec2f(10, 0));
	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> GuiWindow = ofCreateWindow(settings);

	settings.width = 1920;
	settings.height = 1080;
	settings.setPosition(ofVec2f(-1920, 0));
	settings.resizable = false;
	settings.shareContextWith = rtPoseWindow;

	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);


	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<rtPoseApp> rtPoseApp(new rtPoseApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);
	mainApp->rtPose = rtPoseApp;
	mainApp->gui = guiApp;
	guiApp->rtPose = rtPoseApp;
	ofRunApp(rtPoseWindow, rtPoseApp);
	ofRunApp(mainWindow, mainApp);
	ofRunApp(GuiWindow, guiApp);
	ofRunMainLoop();

}
