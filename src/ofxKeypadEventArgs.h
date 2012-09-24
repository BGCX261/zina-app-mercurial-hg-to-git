/*
 *  ofxKeypadEventArgs.h
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFXKEYPAD_EVENT_ARGS
#define _OFXKEYPAD_EVENT_ARGS

#include "ofMain.h"
#include "ofEvents.h"

//class argsKeypadNums : public ofEventArgs
class ofxKeypadEventArgs : public ofEventArgs
{
public:
	int keypadNum;	
};

#endif
