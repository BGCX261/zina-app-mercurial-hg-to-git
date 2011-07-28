/*
 *  VideoPortal.h
 *  zinaApp003
 *
 *  Created by Arne Boon on 3/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIDEO_PORTAL
#define _VIDEO_PORTAL

#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"

#include "ofxDirList.h"

class VideoPortal : public ofBaseApp {
	
public: 
	
	VideoPortal();
	
	void setup(int i);
	void update();
	void draw();
	
	void load(string _keypadString);
	void stop();
	void play();
	void close();
	
	
	void setSize(int _size);
	void setX(int _x);
	void setY(int _y);
	void setOffset(int _offset);
	void updatePosition();
	
	
	int getX();
	int getY();
	int getHeight();
	int getWidth();
	
	EventArgsLogger args;
	ofEvent<EventArgsLogger> onLogEvent;
	
	bool bVideoLoaded;
	bool bVideoPlaying;
	
	
	
private:	
	ofVideoPlayer videoPortal;
	ofxDirList dir;
	
	int x, y, yTemp;
	int w, h;
	int offset;
	
	int videoPortalId;
	
};

#endif