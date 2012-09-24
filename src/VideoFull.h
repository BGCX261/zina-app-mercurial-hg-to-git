/*
 *  VideoFull.h
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIDEO_FULL
#define _VIDEO_FULL

#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"

#include "ofxDirList.h"

#define	NUM_VIDEO_PLAYERS 10

#define VIDEO_PORTAL_LEFT_STRING "left"
#define VIDEO_PORTAL_RIGHT_STRING "right"

class VideoFull : public ofBaseApp {
	
public: 
	
	VideoFull();
	
	void setup();
	void update();
	void draw();
	void playVideoKeyNum(string _keypadNum);
	void playVideoTelNum(string _keypadString);
	
	EventArgsLogger args;
	ofEvent<EventArgsLogger> onLogEvent;
	
	bool bVideoLoaded;
	bool bVideoPlaying;
	bool bVideoMovieDone;
	
private:	
	ofVideoPlayer videoFull;
	ofxDirList dir;
	
	
	
	
	
};

#endif