/*
 *  SerialDevice.cpp
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "SerialDevice.h"
#include <string>

//--------------------------------------------------------------
SerialDevice::SerialDevice()
{
	printf("---SerialDevice constructor---\n");

	bytesRequired = NUM_BYTES;
	bytesRemaining = NUM_BYTES;

	countCycles = 0;
	waitingCycles = 60;
}

//--------------------------------------------------------------
SerialDevice::~SerialDevice()
{
	//
}

//--------------------------------------------------------------
void SerialDevice::setup(string _portName, int _baud)
{
	serial.enumerateDevices();
	//serial.setup("/dev/tty.usbmodem3d11", 9600);
	serial.setup(_portName, _baud);
}

//--------------------------------------------------------------
void SerialDevice::setup(int _deviceId, int _baud)
{
	serial.enumerateDevices();
	serial.setup(_deviceId, _baud);
}


//--------------------------------------------------------------
void SerialDevice::update()
{
	if ( bytesRemaining > 0)
	{
		// check for data
		if ( serial.available() > 0 )
		{
			//reset measurements cycles
			countCycles = 0;

			// try to read - note offset into the bytes[] array, this to not overwrite bytes we already have
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
			countCycles++;
			if(countCycles > waitingCycles) {
				//cout << "...waiting" << endl;
			}
		}
	}

	//--------------------------------------
	else {

		//cout << bytesReturned[0] << endl;
		//cout << bytesReturned << endl;
		
		if (bytesReturned[0] == 'k') {
			//cout << "key = " << bytesReturned[1] << endl;
			argsKeypad.keypadNum = bytesReturned[1];
			ofNotifyEvent(onKeypadPressed, argsKeypad, this);
		}
		
		if (bytesReturned[0] == 'h') {
			//cout << "horn = " << bytesReturned[1] << endl;
			
			bool status = false;
			
			if(bytesReturned[1] == '1') {
				status = true;
			} 
			
			argsHorn.bHornUp = status;
			ofNotifyEvent(onHornStatus, argsHorn, this);
		}
	
		serial.flush();
		bytesRemaining = NUM_BYTES;
	}

}

//--------------------------------------------------------------
void SerialDevice::close() {
	
	serial.flush();
	serial.close();
	serial.enumerateDevices();
	
}
