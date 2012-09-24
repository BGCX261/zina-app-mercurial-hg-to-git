#include "ofMain.h"
#include "zinaApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;

	#ifdef TARGET_WIN32
		ofSetupOpenGL(&window, 1000, 520, OF_WINDOW);
	#else
		ofSetupOpenGL(&window, 1024, 600, OF_WINDOW);
	#endif

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new zinaApp());

}
