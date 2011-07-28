/*
 *  Keypad.cpp
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "ofMain.h"
#include "ofxKeypad.h"

#include <string>

//--------------------------------------------------------------
ofxKeypad::ofxKeypad()
{
	printf("ofxKeypad constructor");

	bytesRequired = NUM_BYTES;
	bytesRemaining = NUM_BYTES;
	
	countCycles = 0;
	waitingCycles = 60;
}

//--------------------------------------------------------------
ofxKeypad::~ofxKeypad()
{
	//
}

//--------------------------------------------------------------
void ofxKeypad::setup(string _portID, int baud)
{
	serial.enumerateDevices();
	//serial.setup("/dev/tty.usbmodem3d11", 9600);
	serial.setup(_portID, baud);
}

//--------------------------------------------------------------
void ofxKeypad::update()
{
	if ( bytesRemaining > 0)
	{
		// check for data
		if ( serial.available() > 0 )
		{
			//reset measurements cycles
			countCycles = 0;
			
			// try to read - note offset into the bytes[] array, this is so
			// that we don't overwrite the bytes we already have
			int bytesArrayOffset = bytesRequired - bytesRemaining;
			int result = serial.readBytes( &bytesReturned[bytesArrayOffset], bytesRemaining );
			
			// check for error code
			if ( result == OF_SERIAL_ERROR )
			{
				// something bad happened
				ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
				cout << "OF_SERIAL_ERROR";			}
			
			else if ( result == OF_SERIAL_NO_DATA )
			{
				// nothing was read, try again
			}
			
			else
			{
				// we read some data!
				bytesRemaining -= result;
			}
		} else {
			//if no serial data available, wait some cycles to give the serial port some time to recover
			//check if notify the tag has been removed and put into waiting position again
			//pass event args
			countCycles++;
			if(countCycles > waitingCycles) {					
				//cout << "...waiting" << endl;
			}
		}
	} 
	
	//--------------------------------------
	else {						
		
		cout << bytesReturned << endl;
		
		ofNotifyEvent(onNewKeypadNum, argsKeypadNums, this);

		
		serial.flush();
		bytesRemaining = NUM_BYTES;
	}
	
}