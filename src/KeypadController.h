/*
 *  KeypadController.h
 *  zinaApp
 *
 *  Created by Arne Boon on 4/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _KEYPAD_CONTROLLER
#define _KEYPAD_CONTROLLER

#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"
#include "ofxTimer.h"
#include "TonesPlayer.h"

class KeypadController : public ofBaseApp {
	
public:
	
	KeypadController();
	~KeypadController();
	
	void setup(int _fontSize, int _x, int _y);
	void update();
	void draw();
	
	void keypadPressed(EventArgsKeypad & args);
	void reset();
	void afterDialDelay();
	
	void setVolume(float _volume);
	void setDailDelay(int _dialDelay);
	
	bool getIsDialingCall();
	
	//--key events
	EventArgsKeypad argsKeypad;
	ofEvent<EventArgsKeypad> onKeypadPressed;
	
	//--log events
	EventArgsLogger argsLogger;
	ofEvent<EventArgsLogger> onLogEvent;
	
	//--call number
	EventArgsCall argsCall;
	ofEvent<EventArgsCall> onCallEvent;
	
	
private:
	
	ofPoint position;
	
	ofTrueTypeFont font;
	ofRectangle fontRectTotal;
	ofRectangle fontRectOneChar;
	
	string dialedNumber;
	string keypadString;
	
	bool bClearStringNextRound;
	
	TimedCounter* blinkCounter;
	bool bBlink;
	int blinkTime;
	
	TimedCounter* dialDelayCounter;
	int dialDelay;
	int dialDelayMin;
	int dialDelayMax;
	
	TonesPlayer dtmfTones;
	
	float volume;
	
	
	
		
};

#endif
