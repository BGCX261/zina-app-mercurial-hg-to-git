/*
 *  AudioComposition.h
 *  zinaApp
 *
 *  Created by Arne Boon on 4/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AUDIO_COMPOSITION
#define _AUDIO_COMPOSITION

#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"
#include "ofMultiDeviceSoundPlayer.h"

class AudioComposition : public ofBaseApp {
	
public: 
	
	AudioComposition();
	~AudioComposition();
	
	void setup(int _stationId);
	void update();
	
	void play();
	void stop();
	
	void setVolume(float _vol);
	
	EventArgsLogger args;
	ofEvent<EventArgsLogger> onLogEvent;
	
private:	
	
	ofMultiDeviceSoundPlayer track;
	
	float volume;
	
	int stationId;
	
	
};

#endif