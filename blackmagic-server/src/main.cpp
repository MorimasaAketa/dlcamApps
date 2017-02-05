#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"
#include "ofAppGLFWWindow.h"

#include "ofxArgParser.h"

//========================================================================
int main(int argc, const char** argv){
	ofxArgParser::init(argc, argv);

	auto app = new ofApp;

	app->device_name = "blackmagic";
	app->port_name = "ipc://bmd00";

	if (ofxArgParser::hasKey("device"))
		app->device_name = ofxArgParser::getValue("device");

	if (ofxArgParser::hasKey("port"))
		app->port_name = ofxArgParser::getValue("port");

	if (ofxArgParser::hasKey("window"))
		app->show_window = ofxArgParser::getValue("window") == "1";

	if (ofxArgParser::hasKey("debug"))
		app->debug = ofxArgParser::getValue("debug") == "1";

	if (app->show_window)
	{
		ofAppGLFWWindow window;
		ofSetupOpenGL(&window, 640, 360, OF_WINDOW);
		ofRunApp(app);
	}
	else
	{
		ofAppNoWindow window;
		ofSetupOpenGL(&window, 640, 360, OF_WINDOW);
		ofRunApp(app);
	}
}
