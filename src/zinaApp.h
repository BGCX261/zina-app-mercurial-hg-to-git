#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxControlPanel.h"
#include "EventArgs.h"
#include "SerialDevice.h"
#include "TonesPlayer.h"
#include "KeypadController.h"
#include "AudioComposition.h"
#include "VideoController.h"
#include "SampleRecorder.h"

#ifdef TARGET_WIN32 
#include <windows.h>
#endif


class zinaApp : public ofBaseApp{

	public:

		//--OF
		void setup();
		void quitApp();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		//--gui
		ofxControlPanel gui;
		void setupGui();
		void eventsIn(guiCallbackData & data);
		void guiEventHandler(guiCallbackData & data);
	
		float appFrameRate;
		ofTrueTypeFont TTF;
	
		simpleLogger loggerMain;
		void onLogEventMain(EventArgsLogger & args);

		//--presentation
		void togglePresentationMode();
		void presentationModeOn();
		void presentationModeOff();
		void toggleMouse();
		void setWindowTopMost();
	
		bool bHideCursor;
		bool bPresentationMode;	
		bool bKeySpacePressed;

		//--serial
		unsigned int serialDeviceId;
		SerialDevice arduino;
		void onKeypadPressed(EventArgsKeypad & args);
		void onHornStatus(EventArgsHorn & args);
	
		//--KeypadController
		KeypadController keypadController;
		void onCallEvent(EventArgsCall & args);

		//--text
		ofTrueTypeFont textFont;
		string instructionString;
		string callString;
		string returnTextString;

		//--video
		VideoController videoController;

		//--audio composition
		AudioComposition audioComposition;
		ofxSoundStream voiceInputStream;
	
		//--disconnectTone
		ofSoundPlayer disconnectTone;
	
		//--recording
		SampleRecorder sampleRecorder;
		void onFinishedRecordingEvent(EventArgsRecording & args);
		void onFinishedIntroductionEvent(EventArgsIntroductionRecording & args);
	
		//--utility functions
		static void splitString(const string& s, char c, vector<string>& v);
		static void drawShadedString( ofTrueTypeFont& _font, const string& _s, float _x,
										float _y, int _offsetX = 1, int _offsetY = 1 );
		static void drawShadedString( ofTrueTypeFont& _font, const string& _s, float _x,
										float _y, ofColor _fgColor, ofColor _bgColor, int _offsetX = 1, int _offsetY = 1 );
		
	
	private:
	
		static const string messageNumber;
	
		bool bShowFPS;
		bool bRecordingOptionOn;
	
		ofxDirList dirListStations;
		vector <string> vectorStationIds;
	
		vector <string> vectorSerialIds;
	
		vector<int> asciiKeys;

};

#endif


/*
hack in VideoPlayer: line 369: comment this:

        // if (width != 0 && height != 0){
		//	bDoWeAlreadyHaveAGworld = true;
		// }

FIX: doing this check makes that a VideoPlayer instance can't load a second, third video

*/
