/*
 *  KeypadController.cpp
 *  zinaApp
 *
 *  Created by Arne Boon on 4/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "KeypadController.h"
#include "ofMain.h"
#include <string>

//--------------------------------------------------------------
KeypadController::KeypadController()
{
	printf("---KeypadController constructor---\n");

	keypadString = "";
	bClearStringNextRound = false;
	bBlink = false;
	blinkTime = 10;
	
	dialDelay = 50;
	dialDelayMin = 40;
	dialDelayMax = 60;
}

//--------------------------------------------------------------
KeypadController::~KeypadController()
{
	//
}

//--------------------------------------------------------------
void KeypadController::setup(int _fontSize, int _x, int _y)
{
	//--position
	position.x = _x;
	position.y = _y;
	
	//--font
	font.loadFont("MONACO.TTF", _fontSize, true, true, false);

	//--font bounding box
	keypadString = "0";
	fontRectOneChar = font.getStringBoundingBox(keypadString, 0, 0);
	keypadString = "";
	fontRectTotal = font.getStringBoundingBox(keypadString, 0, 0);
	
	//--blinker
	this->blinkCounter = new TimedCounter(blinkTime);
	this->blinkCounter->startCount();
	
	dialDelay = (int) ofRandom(dialDelayMin, dialDelayMax);
	this->dialDelayCounter = new TimedCounter(dialDelay);
	
	//--dtmf
	dtmfTones.setup();

}

//--------------------------------------------------------------
void KeypadController::update()
{

	fontRectTotal = font.getStringBoundingBox(keypadString, 0, 0);
	
	//--blink-blinkCounter
	if(this->blinkCounter->isCounting()){
		this->blinkCounter->update();
		if(this->blinkCounter->isCountComplete()) {
			bBlink = !bBlink;
			//this->blinkCounter = new TimedCounter(blinkTime);
			this->blinkCounter->endCount();
			this->blinkCounter->nextCount();
			this->blinkCounter->startCount();
		}
	}
	
	//--dialDelay-counter
	if(this->dialDelayCounter->isCounting()){
		this->dialDelayCounter->update();
		if(this->dialDelayCounter->isCountComplete()) {
			this->dialDelayCounter->endCount();
			afterDialDelay();
			dialDelay = (int) ofRandom(dialDelayMin, dialDelayMax);
			//this->dialDelayCounter = new TimedCounter(dialDelay);
			this->dialDelayCounter->nextCount();
		}
	}
	
	//--dtmf
	dtmfTones.update();
}

//--------------------------------------------------------------
void KeypadController::draw()
{
	//--cursor
	if (bBlink == true) {
		
		int offsetCursor = 0;
		if (fontRectTotal.width != 0) {
			offsetCursor = 15;
		}
			
		//NOTE: subtract 4 from rectangle height to have the cursor line up with the bottom thumb video
		ofRect(position.x + fontRectTotal.width + offsetCursor,
			   position.y - fontRectOneChar.height + 8,
			   fontRectOneChar.width, 
			   fontRectOneChar.height - 4);

	}
	
	//--string
	font.drawString(keypadString, position.x, position.y);
	
	
}

//--------------------------------------------------------------
void KeypadController::keypadPressed(EventArgsKeypad & args)
{	
	//--dtmf
	dtmfTones.playNum( args.keypadNum );
	
	//--
	if (args.keypadNum == '*' || args.keypadNum == '#') {
		if (keypadString.length() >= 1) {
			keypadString.erase( keypadString.length()-1, 1 );
		}
	} else {
		keypadString.push_back(args.keypadNum);
	}
	
	//--
	if(keypadString.length() >= 10) {
		//--protect the dialed number from user change during the delay in order to pass it in the event in afterDialDelay()
		dialedNumber = keypadString;
		dtmfTones.playDialTone();
		this->dialDelayCounter->startCount();
	}
	
	if (keypadString.length() >= 11) {
		keypadString.clear();
		keypadString.push_back(args.keypadNum);
	}
	
}

//--------------------------------------------------------------
void KeypadController::reset()
{
	keypadString.clear();
}
	
//--------------------------------------------------------------
void KeypadController::afterDialDelay()
{
	
	//event
	argsCall.calledNumber = dialedNumber;
	ofNotifyEvent(onCallEvent, argsCall, this);
	
	//--stop dial tone
	dtmfTones.stopDialTone();
	
	//--empty string
	keypadString.clear();
}


//------------------------------------------
void KeypadController::setVolume(float _volume)
{
	volume = _volume;
	
	dtmfTones.setVolume(volume);
}

//------------------------------------------
void KeypadController::setDailDelay(int _dialDelay)
{
	dialDelay = _dialDelay;
	dialDelayMin = (int) _dialDelay * 0.7;
	dialDelayMax = (int) _dialDelay * 1.3;
	
	dialDelay = (int) ofRandom(dialDelayMin, dialDelayMax);
	this->dialDelayCounter = new TimedCounter(dialDelay);
}

//------------------------------------------
bool KeypadController::getIsDialingCall()
{
	if (this->dialDelayCounter->isCounting() == true) {
		cout << "CALLING... NO KEY INPUT PLEASE" << endl;
		return true;
	} else {
		return false;
	}
}



	

	
	
	
	
	
	
	
	
