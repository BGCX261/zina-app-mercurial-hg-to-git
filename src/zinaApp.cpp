#include "zinaApp.h"

#include <string>
#include <vector>
#include <functional>
#include <iostream>

#define SOUND_INPUT_SAMPLERATE 44100
#define SOUND_INPUT_BUFFERSIZE 256

using namespace std;

/***********************************************
 * TODO
 *
 * - probably best to pause VideoController when recording starts, and restart it
 *   when recording stops (maybe add MODE_STOP to VideoController?)
 * - app crashes in RtAudio.h (??) on exit (at least on osx)
 * - setting window top most works, but bringing focus back on this window does not work
 *
 */

const string zinaApp::messageNumber = "0900123490";

//--------------------------------------------------------------
//-- ZINA APP --------------------------------------------------
//--------------------------------------------------------------
void zinaApp::setup(){
	cout << "---zinaApp---" << endl;

	//--of
	ofSetWindowTitle("ZinaApp");
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	//--gui-vars--------------------------------------
	bHideCursor = true;
	bPresentationMode = false;
	appFrameRate = 0;
	bKeySpacePressed = false;
	bRecordingOptionOn = true;
	
	//--gui-setup
	setupGui();

	//--arduino-----------------------------------
	//serialDeviceId = 5; //TODO: from GUI (now: windows = 0, mac arne = 5, mac wouter = 10)
	int newSerialId = gui.getValueI("SERIAL_ID", 0);
	#ifdef TARGET_WIN32  
		newSerialId = 0; //windows
	#endif
	arduino.setup(newSerialId, 9600);
	cout << "SERIAL >>> setup device " << newSerialId << endl;
	ofAddListener(arduino.onKeypadPressed, this, &zinaApp::onKeypadPressed);
	ofAddListener(arduino.onHornStatus, this, &zinaApp::onHornStatus);
	
	//--keypadController--------------------------
	keypadController.setup(56, 32, 564);
	keypadController.setVolume( gui.getValueF("VOLUME_DIAL_TONES", 0));
	keypadController.setDailDelay( gui.getValueI("DIAL_DELAY", 0));
	ofAddListener(keypadController.onCallEvent, this, &zinaApp::onCallEvent);

	//--font--instruction
	textFont.loadFont("MONACO.TTF", 15, true, true, false);
	instructionString = "Toets telefoonnummer om verhaal te starten";
	stringstream s;
	s << "Bel " << messageNumber << " om boodschap in te spreken";
	callString = s.str();
	returnTextString = "Nummer wissen: toets #";

	//--video---------------------------------------
	int newStationId = gui.getValueI("STATION_ID", 0) + 1;
	videoController.setup( newStationId );
	videoController.setVolumeThumbs( gui.getValueF("VOLUME_THUMBS", 0) );
	videoController.setVolumeFull( gui.getValueF("VOLUME_FULL", 0) );
	videoController.setShowMinutesPortal( gui.getValueB("PORTAL_SHOW_MINUTES", 0));

	//--audio composition-------------------------------------
	audioComposition.setup( newStationId );
	audioComposition.setVolume( gui.getValueF("VOLUME_AURA", 0));

	//--voice recording------------------------------
	voiceInputStream.setDeviceId( 0 );
	voiceInputStream.setup( 0, 2, this, SOUND_INPUT_SAMPLERATE, SOUND_INPUT_BUFFERSIZE, 4 );
	sampleRecorder.setup( voiceInputStream , gui.getValueI("RECORDING_DURATION", 0) );
	sampleRecorder.setVolumeRecIntro( gui.getValueF("VOLUME_REC_INTRO", 0) );
	ofAddListener(sampleRecorder.onFinishedRecordingEvent, this, &zinaApp::onFinishedRecordingEvent);
	ofAddListener(sampleRecorder.onFinishedIntroductionEvent, this, &zinaApp::onFinishedIntroductionEvent );
	
	//--disconnect tone------------------------------
	disconnectTone.setMultiPlay(false);
	disconnectTone.stop();
	disconnectTone.bLoop = false;
	disconnectTone.setVolume( gui.getValueF("VOLUME_DIAL_TONES", 0));
	disconnectTone.loadSound("sound/disconnectTone/disconnectTone.wav");
	
	//--fps------------------------------------------
	bShowFPS = false;
	
	//--gui setting----------------------------------
	bRecordingOptionOn = gui.getValueB("VOICE_RECORDING_OPTION", 0);
	
	//--gui------------------------------------------
	#ifdef TARGET_WIN32
		this->presentationModeOn();
	#else
		gui.hide();
		bShowFPS = true;
	#endif
	
	//--ascii-- 48-57= 0-9
	for (int i = 48; i < 58; i++) {
		asciiKeys.push_back(i);
	}
}

//--------------------------------------------------------------
//-- UPDATE ----------------------------------------------------
//--------------------------------------------------------------
void zinaApp::update(){

	//--of
	ofBackground(0, 0, 0);

	//--serial
	arduino.update();
	
	//--keypad
	keypadController.update();
	
	//--video
	videoController.update();
	
	//--audio composition
	audioComposition.update();
	
	//--sample recorder
	sampleRecorder.update();
	
	//--topmost
	if(ofGetFrameNum() % 100 == 0) {
		setWindowTopMost();
	}

	//--gui
	appFrameRate = ofGetFrameRate();
	gui.update();
}

//--------------------------------------------------------------
//-- DRAW ------------------------------------------------------
//--------------------------------------------------------------
void zinaApp::draw(){

	//--set color
	ofSetColor(255, 255, 255);

	if ( ! sampleRecorder.bInRecordingModus ) {
		//--check video mode
		if( videoController.getMode() == VideoController::VM_PORTAL ) {
		
			//--text
			textFont.drawString(instructionString, 32, 424);
			textFont.drawString(returnTextString, 32, 454);
			
			if (bRecordingOptionOn) {
				textFont.drawString(callString, 32, 484);
			}

			//--keypadController
			keypadController.draw();
		}
		
		//--video
		videoController.draw();
	} else {
		
		sampleRecorder.draw();
	}

	//--fps
	if (bShowFPS) {
		textFont.drawString( ofToString(appFrameRate, 0), 2, 20);
	}
	
	
	//--gui
	ofSetColor(255);
	gui.draw();
}

//--------------------------------------------------------------
//-- HORN STATUS ------------------------------------------------
//--------------------------------------------------------------
void zinaApp::onHornStatus(EventArgsHorn & args) {
	
	if (args.bHornUp == false) {
		loggerMain.log(OF_LOG_NOTICE, "%s", "ON HORN STATUS >> B HORN UP false");
		audioComposition.play();
		
		//--go back to portal mode and stop recording
		if ( ! sampleRecorder.bInRecordingModus ) {
			if (videoController.getMode() == VideoController::VM_FULL) {
				videoController.setMode(VideoController::VM_PORTAL);
			}
		} else {
			sampleRecorder.stopRecording();
		}
		
		//--keypad string to empty again
		keypadController.reset();
	}
	
	if (args.bHornUp == true) {
		loggerMain.log(OF_LOG_NOTICE, "%s", "ON HORN STATUS >> B HORN UP true");
		audioComposition.stop();
	}
}

//--------------------------------------------------------------
//-- KEYPAD ----------------------------------------------------
//--------------------------------------------------------------
void zinaApp::onKeypadPressed(EventArgsKeypad & args) {
	if ( ! sampleRecorder.bInRecordingModus ) {
		
		if (videoController.getMode() == VideoController::VM_FULL) {
			if (args.keypadNum == '#') {
				videoController.setMode(VideoController::VM_PORTAL);
				keypadController.keypadPressed(args);	//passing serial input to keypad controller, trigger sound '#'
			} 
		} else {
			if (keypadController.getIsDialingCall() == false) {
				
				//--MOST IMPORTANT: pass key input from serial to key handling in keypadController
				keypadController.keypadPressed(args);
			}
			
		}
	} else {	
		if (args.keypadNum == '#') {
			sampleRecorder.stopRecording();
			keypadController.keypadPressed(args);
		}
	}
}

//--------------------------------------------------------------
//-- KEYPAD ----------------------------------------------------
//--------------------------------------------------------------
void zinaApp::onCallEvent(EventArgsCall & args) {
	if ( args.calledNumber.compare( messageNumber ) == 0 ) {

		cout << "zinaApp::onCallEvent >> message number found" << endl;
		cout << "zinaApp -> bRecordingOption = " << ofToString(bRecordingOptionOn) << " (starting recording modus if 1) " << endl;
		
		if (bRecordingOptionOn) {
			sampleRecorder.startIntroduction();
			//sampleRecorder.startRecording();
			videoController.setVolumeThumbs(0.0);
		} else {
			disconnectTone.play();
		}
		
	} else {
		bool result = videoController.prepareVideoForFullMode( args.calledNumber );
		
		cout << "zinaApp::onCallEvent:: RESULT = " << result << " for '" << args.calledNumber << "' " << endl;
		
		if (result) {
			videoController.setMode(VideoController::VM_FULL);
		} else {
			disconnectTone.play();
			cout << "zinaApp::onCallEvent:: no number found -> play disconnect tone" << endl;
		}
	}
}

//--------------------------------------------------------------
//-- RECORDING EVENT ----------------------------------------------------
//--------------------------------------------------------------
void zinaApp::onFinishedRecordingEvent(EventArgsRecording & args) {
	
	videoController.setVolumeThumbs( gui.getValueF("VOLUME_THUMBS", 0) );
	cout << "set volume back to gui value" << endl;
}

//--------------------------------------------------------------
void zinaApp::onFinishedIntroductionEvent(EventArgsIntroductionRecording & args) {
	
	cout << "GOT IT: startRecording()" << endl;
	sampleRecorder.startRecording();
	
}

//--------------------------------------------------------------
//-- GUI -------------------------------------------------------
//--------------------------------------------------------------
void zinaApp::setupGui(){

	//--gui--begin
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 150));
	ofxControlPanel::setTextColor(simpleColor(220, 50, 50, 255));

	gui.loadFont("MONACO.TTF", 12);
	gui.setup("ZinaApp", 10, 10, ofGetWidth() - 20, ofGetHeight() - 40, true);

	//--gui--panel0--------------------------------
	gui.addPanel("mainPanel", 5, false);
	gui.setWhichPanel(0);
	gui.setWhichColumn(0);

	gui.addChartPlotter("appFrameRate", guiStatVarPointer("app fps", &appFrameRate, GUI_VAR_FLOAT, true, 2), 300, 100, 200, 0, 70);

	vector <guiVariablePointer> vars;
	vars.push_back( guiVariablePointer("appFrameRate", &appFrameRate, GUI_VAR_FLOAT, 0) );
	gui.addVariableLister("app vars", vars);
	
	//--
	gui.addLabel("OPTIONAL SETTINGS");

	gui.addToggle("Voice Recording On", "VOICE_RECORDING_OPTION", true);
	ofAddListener(gui.createEventGroup("VOICE_RECORDING_OPTION"), this, &zinaApp::guiEventHandler);
	
	gui.addToggle("Show Minutes Portal On", "PORTAL_SHOW_MINUTES", true);
	ofAddListener(gui.createEventGroup("PORTAL_SHOW_MINUTES"), this, &zinaApp::guiEventHandler);

	//--
	gui.addLabel("APP SETTINGS");
	
	gui.addToggle("togglePresentation (p)", "TOGGLE_PRESENTATION", 0);
	ofAddListener(gui.createEventGroup("TOGGLE_PRESENTATION"), this, &zinaApp::guiEventHandler);

	gui.addToggle("Quit Application (q)", "QUIT_APPLICATION", 0);
	ofAddListener(gui.createEventGroup("QUIT_APPLICATION"), this, &zinaApp::guiEventHandler);
	
	
	
	//--column
	gui.setWhichColumn(2);

	gui.addSlider("Volume Full", "VOLUME_FULL", 40.0, 0.0, 100.0, true);
	ofAddListener(gui.createEventGroup("VOLUME_FULL"), this, &zinaApp::guiEventHandler);
	
	gui.addSlider("Volume Thumbs", "VOLUME_THUMBS", 10.0, 0.0, 100.0, true);
	ofAddListener(gui.createEventGroup("VOLUME_THUMBS"), this, &zinaApp::guiEventHandler);
	
    gui.addSlider("Volume Aura", "VOLUME_AURA", 0.75, 0.0, 1.0, false);
	ofAddListener(gui.createEventGroup("VOLUME_AURA"), this, &zinaApp::guiEventHandler);
	
	gui.addSlider("Volume Dial Tones", "VOLUME_DIAL_TONES", 0.15, 0.0, 1.0, false);
	ofAddListener(gui.createEventGroup("VOLUME_DIAL_TONES"), this, &zinaApp::guiEventHandler);
	
	gui.addSlider("DialDelay", "DIAL_DELAY", 50.0, 10.0, 300.0, true);
	ofAddListener(gui.createEventGroup("DIAL_DELAY"), this, &zinaApp::guiEventHandler);
	
	gui.addSlider("Volume Rec Intro", "VOLUME_REC_INTRO", 0.30, 0.0, 1.0, false);
	ofAddListener(gui.createEventGroup("VOLUME_REC_INTRO"), this, &zinaApp::guiEventHandler);
	
	gui.addSlider("Recording Duration", "RECORDING_DURATION", 90.0, 30.0, 160.0, true);
	ofAddListener(gui.createEventGroup("RECORDING_DURATION"), this, &zinaApp::guiEventHandler);

	
	//--column
	gui.setWhichColumn(4);
	
	//--station id
	for (int i =  0; i < dirListStations.listDir("stations/"); i++) {
		vectorStationIds.push_back( dirListStations.getName(i) );
	}
	
	gui.addTextDropDown("Select station ID", "STATION_ID", 0, vectorStationIds);
	ofAddListener(gui.createEventGroup("STATION_ID"), this, &zinaApp::guiEventHandler);

	//--serial id
	for (int i = 0; i < 16; i++) {
		vectorSerialIds.push_back( "Serial Device " + ofToString(i) );
	}
	
	gui.addTextDropDown("Select serial ID", "SERIAL_ID", 0, vectorSerialIds);
	ofAddListener(gui.createEventGroup("SERIAL_ID"), this, &zinaApp::guiEventHandler);
	
	
	//--gui--panel1--------------------------------
	gui.addPanel("loggerMainPanel", 4, false);
	gui.setWhichPanel(1);
	gui.setWhichColumn(0);

	gui.addLogger("loggerMain", &loggerMain, ofGetWidth() - 80, ofGetHeight() - 150);

	//--gui-end
	gui.loadSettings("controlPanelSettings.xml");
	gui.setupEvents();
	gui.enableEvents();

	//ofAddListener(gui.guiEvent, this, &zinaApp::eventsIn);	//shows all gui events
}

//--------------------------------------------------------------
void zinaApp::guiEventHandler(guiCallbackData & data){

	if (data.isElement( "PLACEHOLDER_FIRST_IF")) {
		//empty
	}
	
	else if( data.isElement( "TOGGLE_PRESENTATION" ) && data.getInt(0) == 1 ){
		gui.setValueB("QUIT_APPLICATION", false);
		this->togglePresentationMode();
	}
	
	else if( data.isElement( "PORTAL_SHOW_MINUTES" ) ){
		videoController.setShowMinutesPortal( ! videoController.getShowMinutesPortal() );
		gui.setValueB("PORTAL_SHOW_MINUTES", videoController.getShowMinutesPortal());
	}

	else if( data.isElement( "QUIT_APPLICATION" ) && data.getInt(0) == 1 ){
		gui.setValueB("QUIT_APPLICATION", false);
		this->quitApp();
	}
	
	else if( data.isElement( "RECORDING_DURATION" ) ){
		gui.saveSettings(); 
		// TODO: update to sampleRecorder.setup(): but not each move in gui, only when final selection is made: oterhwise lots of resets while dragging
	}
	
	else if( data.isElement( "VOICE_RECORDING_OPTION" ) ){
		bRecordingOptionOn = !bRecordingOptionOn;
		gui.setValueB("VOICE_RECORDING_OPTION", bRecordingOptionOn);
	}

	else if( data.isElement( "VOLUME_THUMBS" ) ){
		videoController.setVolumeThumbs( data.getInt(0) );
	}
	
	else if( data.isElement( "VOLUME_FULL" ) ){
		videoController.setVolumeFull( data.getInt(0) );
	}
	
	else if( data.isElement( "VOLUME_AURA" ) ){
		audioComposition.setVolume( data.getFloat(0) );
	}
	
	else if( data.isElement( "VOLUME_DIAL_TONES" ) ){
		keypadController.setVolume( data.getFloat(0) );
		disconnectTone.setVolume( data.getFloat(0) );
	}
	
	else if( data.isElement( "VOLUME_REC_INTRO" ) ){
		sampleRecorder.setVolumeRecIntro( data.getFloat(0) );
	}
	
	else if( data.isElement( "DIAL_DELAY" ) ){
		keypadController.setDailDelay( data.getInt(0) );
	}
	
	else if( data.isElement( "STATION_ID" ) ){
		gui.saveSettings();
		
		this->quitApp();
		
		//int newStationId = data.getInt(0) + 1;
		//videoController.setup(newStationId);
		//audioComposition.setup(newStationId);
	}
	
	else if( data.isElement( "SERIAL_ID" ) ){
		int newSerialId = data.getInt(0);
		
		arduino.close();
		arduino.setup(newSerialId, 9600);
		cout << "SERIAL >>> setup device " << newSerialId << endl;
	}

	//--gui-save settings--------------------
	gui.saveSettings();
	loggerMain.log(OF_LOG_NOTICE, "%s", "GUI >> SAVE SETTINGS");
	//cout << "ON_LOG_EVENT_MAIN >> GUI >> SAVE SETTINGS" << endl;

}

//--------------------------------------------------------------
void zinaApp::eventsIn(guiCallbackData & data){
	
	//lets send all events to our logger
	if( !data.isElement( "events logger" ) ){
		string logStr = data.getXmlName();
		
		for(int k = 0; k < data.getNumValues(); k++){
			logStr += " - " + data.getString(k);
		}
		
		loggerMain.log(OF_LOG_NOTICE, "event - %s", logStr.c_str());
	}
	
	// print to terminal if you want to
	printf("zinaApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
	if( data.getDisplayName() != "" ){
		printf(" element name is %s \n", data.getDisplayName().c_str());
	}
	for(int k = 0; k < data.getNumValues(); k++){
		if( data.getType(k) == CB_VALUE_FLOAT ){
			printf("%i float value = %f \n", k, data.getFloat(k));
		}
		else if( data.getType(k) == CB_VALUE_INT ){
			printf("%i int value = %i \n", k, data.getInt(k));
		}
		else if( data.getType(k) == CB_VALUE_STRING ){
			printf("%i string value = %s \n", k, data.getString(k).c_str());
		}
	}
	
	printf("\n");
}

//--------------------------------------------------------------
void zinaApp::drawShadedString( ofTrueTypeFont& _font, const string& _s, float _x, float _y, int _offsetX, int _offsetY ) {
	ofColor fg, bg;
	fg.r = fg.g = fg.b = fg.a = 250;
	bg.r = bg.g = bg.b = bg.a = 0;
	drawShadedString( _font, _s, _x, _y, fg, bg, _offsetX, _offsetY );
}

//--------------------------------------------------------------
void zinaApp::drawShadedString( ofTrueTypeFont& _font, const string& _s, float _x, float _y, ofColor _fgColor, ofColor _bgColor, int _offsetX, int _offsetY ) {
		
	/* //NOTICE: takes fps minus 3-5 fps
	ofSetColor( _bgColor.r, _bgColor.g, _bgColor.b );
	_font.drawString( _s, _x + _offsetX, _y + _offsetY );*/
	
	//NOTICE: takes minus 2-3 fps 
	ofSetColor(0, 0, 0);
	ofRectangle rect = _font.getStringBoundingBox(_s, 0, 0);
	float offset = 3.0;
	ofRect(_x - offset, _y - rect.height + offset, rect.width + offset*2, rect.height + offset*2);
		
	ofSetColor( _fgColor.r, _fgColor.g, _fgColor.b );
	_font.drawString( _s, _x, _y );
	
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
//-- CUSTOM HANDLERS -------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
void zinaApp::quitApp() {
	cout << "zinaApp::exit()" << endl;
	std::exit(0);
}

//--------------------------------------------------------------
void zinaApp::splitString(const string& s, char c, vector<string>& v) {
	string::size_type i = 0;
	string::size_type j = s.find(c);
	
	while (j != string::npos) {
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);
		
		if (j == string::npos) {
			v.push_back(s.substr(i, s.length()));
		}
	}
}

//--------------------------------------------------------------
void zinaApp::onLogEventMain(EventArgsLogger & args) {
	loggerMain.log(OF_LOG_NOTICE, "%s", args.sLog.c_str());
	
	//cout << "ON_LOG_EVENT_MAIN >> " << args.sLog.c_str() << endl;
}

//--------------------------------------------------------------
//-- INPUT HANDLERS KEY, MOUSE, WINDOW -------------------------
//--------------------------------------------------------------
void zinaApp::keyPressed(int key){
	
	//----------------------------------------------
	//--bKeySpacePressed: gui key's only work if SPACE is pressed
	if (key == ' ') {
		bKeySpacePressed = true;
	}

	if (bKeySpacePressed == true) {
		
		//--debug with keyboard-------------------------------------
		EventArgsKeypad keyboardInput;
		
		//-ascii 0-9
		for (int i = 0; i < (int) asciiKeys.size(); i++) {
			if (key == asciiKeys[i]) {
				keyboardInput.keypadNum = key;
				onKeypadPressed( keyboardInput );
			}
		}
		
		//-ascii * # placed at - and +
		if (key == '-' || key == '_' ) {
			keyboardInput.keypadNum = '*';
			onKeypadPressed( keyboardInput );
		}
		if (key == '=' || key == '+' ) {
			keyboardInput.keypadNum = '#';
			onKeypadPressed( keyboardInput );
		}
		
		//-GUI KEYS--------------------------------------------------
		
		if (key == 'f' || key == 'F') {
			ofToggleFullscreen();
		}
		
		if (key == 'g' || key == 'G') {
			this->gui.toggleView();
			this->toggleMouse();
		}
		
		if (key == 'q' || key == 'Q') {
			this->quitApp();
		}
		
		if (key == 'p' || key == 'P' ) {
			this->togglePresentationMode();
		}
		
		if (key == 'r' || key == 'R' ) {
			bShowFPS = !bShowFPS;
		}
		
		if (key == 'm' || key == 'M' ) {
			videoController.setShowMinutesPortal( ! videoController.getShowMinutesPortal() );
			gui.setValueB("PORTAL_SHOW_MINUTES", videoController.getShowMinutesPortal());
		}
		
		if (key == 'o' || key == 'O') {
			bRecordingOptionOn = !bRecordingOptionOn;
			gui.setValueB("VOICE_RECORDING_OPTION", bRecordingOptionOn);
		}
	}

}

//--------------------------------------------------------------
void zinaApp::keyReleased(int key){
	
	if (key == ' ') {
		bKeySpacePressed = false;
	}

}

//--------------------------------------------------------------
void zinaApp::togglePresentationMode(){
	if(bPresentationMode == true) {
		presentationModeOff();
	} else {
		presentationModeOn();
	}
}

//--------------------------------------------------------------
void zinaApp::presentationModeOn(){
	
	ofSetFullscreen(true);
	ofHideCursor();
	bHideCursor = true;
	gui.hide();
	
	bPresentationMode = true;
	
	setWindowTopMost();
}

//--------------------------------------------------------------
void zinaApp::setWindowTopMost() {
	//--TODO: make window really active when other window was selected
	
	#ifdef TARGET_WIN32  

		//get its handle "GLUT" = class name "ogl" = window   
		HWND hwnd = FindWindow( "GLUT", "zinaApp" );   
		//set the window always-on-top  
		SetWindowPos( hwnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOREPOSITION | SWP_NOSIZE );  
	
		//--none of below are working really..
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);
		SetActiveWindow(hwnd);
		
	#endif  
}
	
//--------------------------------------------------------------
void zinaApp::presentationModeOff(){

	ofSetFullscreen(false);
	bPresentationMode = false;
}

//--------------------------------------------------------------
void zinaApp::toggleMouse(){

	if(bHideCursor) {
		gui.setValueB("TOGGLE_MOUSE", false);
		bHideCursor = false;
		ofShowCursor();
	} else {
		gui.setValueB("TOGGLE_MOUSE", true);
		bHideCursor = true;
		ofHideCursor();
	}
}

//--------------------------------------------------------------
void zinaApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void zinaApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void zinaApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void zinaApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased();
}

//--------------------------------------------------------------
void zinaApp::windowResized(int w, int h){

}

