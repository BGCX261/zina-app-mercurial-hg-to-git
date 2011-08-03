/*
 *  EventArgs.h
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EVENT_ARGS
#define _EVENT_ARGS

#include "ofMain.h"
#include "ofEvents.h"

//class EventArgsRFID : public ofEventArgs
//{
//public:
//	int portID;
//	string tagID;
//	
//};

class EventArgsLogger : public ofEventArgs
{
public:
	string sLog;
	
};

class EventArgsKeypad : public ofEventArgs
{
public:
	unsigned char keypadNum;	
};

class EventArgsHorn : public ofEventArgs
{
public:
	bool bHornUp;	
};

class EventArgsCall : public ofEventArgs
{
public:
	string calledNumber;
};

class EventArgsRecording : public ofEventArgs
{
public:
	bool bIsRecording;
};

class EventArgsIntroductionRecording : public ofEventArgs
{
public:
	bool bFinishedIntroduction;
};

#endif