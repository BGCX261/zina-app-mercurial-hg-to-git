/*
 *  serialDevice.h
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SERIAL_DEVICE
#define _SERIAL_DEVICE

#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"

#define NUM_BYTES	2				//number of bytes to read as one message

class SerialDevice : public ofBaseApp {

	public:

	SerialDevice();
	~SerialDevice();

	void setup(string _portName, int _baud);
	void setup(int _deviceId, int _baud);
	void update();
	
	void close();

	//--key events
	EventArgsKeypad argsKeypad;
	ofEvent<EventArgsKeypad> onKeypadPressed;
	
	//--ldr events
	EventArgsHorn argsHorn;
	ofEvent<EventArgsHorn> onHornStatus;

	//--log events
	EventArgsLogger argsLogger;
	ofEvent<EventArgsLogger> onLogEvent;

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
