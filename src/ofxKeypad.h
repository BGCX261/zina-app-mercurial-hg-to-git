/*
 *  Keypad.h
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _OFXKEYPAD
#define _OFXKEYPAD

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxKeypadEventArgs.h"

#define NUM_BYTES					1				//number of bytes in sent by the rfid reader (for Parallax readers: 12 bytes)			

class ofxKeypad : public ofBaseApp {

	public:
	
		ofxKeypad();
		~ofxKeypad();

		void setup(string _portName, int _portID);
		void update();
	
		ofxKeypadEventArgs argsKeypadNums;
		ofEvent<ofxKeypadEventArgs> onNewKeypadNum;
		

	private:
	
		//--ofSerial
		ofSerial serial;
		
		//--vars
		unsigned char bytesReturned[NUM_BYTES];
		int bytesRequired;
		int bytesRemaining;
		int countCycles;
		
		int waitingCycles;
	
};

#endif