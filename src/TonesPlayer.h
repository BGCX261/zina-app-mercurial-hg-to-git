/*
 *  tonesPlayer.h
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TONES_PLAYER
#define _TONES_PLAYER

#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"

#define	NUM_SOUND_PLAYERS 12

class TonesPlayer : public ofBaseApp {
	
public: 
	
	TonesPlayer();
	
	void setup();
	void update();
	void playNum(unsigned char _keypadNum);
	void playDialTone();
	void stopDialTone();
	
	bool bDailToneIsPlaying;
	
	void setVolume(float volume);
	
	EventArgsLogger args;
	ofEvent<EventArgsLogger> onLogEvent;
	
private:	
	
	ofSoundPlayer tonesPlayers[NUM_SOUND_PLAYERS];
	
	ofSoundPlayer dialTone;
	
	float volume;

	
};

#endif