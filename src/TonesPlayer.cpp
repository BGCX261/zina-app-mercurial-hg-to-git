/*
 *  tonesPlayer.cpp
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "tonesPlayer.h"

#include "ofMain.h"
#include <iostream>


//--------------------------------------------------------------
TonesPlayer::TonesPlayer()
{
	printf("---TonesPlayer constructor--- \n");
}

//--------------------------------------------------------------
void TonesPlayer::setup(){

	for (int i = 0; i < NUM_SOUND_PLAYERS; i++) {
		tonesPlayers[i].setMultiPlay(false);
		tonesPlayers[i].stop();
		tonesPlayers[i].bLoop = false;
		tonesPlayers[i].loadSound( "sound/dtmfTones/DTMF-" + ofToString(i) + ".wav");
	}

	dialTone.setMultiPlay(false);
	dialTone.stop();
	dialTone.bLoop = true;
	dialTone.loadSound("sound/dtmfTones/DTMF-dialTone.wav");
	//dialTone.setVolume(0.7);

	bDailToneIsPlaying = false;
}

//--------------------------------------------------------------
void TonesPlayer::update() {
	ofSoundUpdate();
}

//--------------------------------------------------------------
void TonesPlayer::playNum(unsigned char _keypadNum) {
	int num = 0;
	
	if (!isdigit(_keypadNum) && _keypadNum != '*' && _keypadNum != '#') {
		
		args.sLog = "TONES PLAYER >>> PLAY >>> NOT a digit!!";
		ofNotifyEvent(onLogEvent, args, this);
		
		return;
	}

	if (_keypadNum == '*') {
		num = 10;
	} else if (_keypadNum == '#') {
		num = 11;
	} else {
		num = _keypadNum - '0'; //ascii 48 == 0
	}
	
	tonesPlayers[num].play();
	args.sLog = "TONES PLAYER >>> PLAY >>> KeypadNum: " + ofToString(num);
	ofNotifyEvent(onLogEvent, args, this);

}

//--------------------------------------------------------------
void TonesPlayer::playDialTone() {
	/** NOTE: add ofRandom in parameters: loop dial tone 1 - 4 times **/

	//cout << "tones player: play: dialTone" << endl;

	dialTone.play();
	bDailToneIsPlaying = true;

	args.sLog = "TONES PLAYER >>> PLAY >>> dialTone";
	ofNotifyEvent(onLogEvent, args, this);
}

//--------------------------------------------------------------
void TonesPlayer::stopDialTone() {
	/** NOTE: add ofRandom in parameters: loop dial tone 1 - 4 times **/

	//cout << "tones player: play: dialTone" << endl;

	dialTone.stop();
	bDailToneIsPlaying = false;

	args.sLog = "TONES PLAYER >>> STOP >>> dialTone";
	ofNotifyEvent(onLogEvent, args, this);
}

//--------------------------------------------------------------
void TonesPlayer::setVolume(float _volume) {

	volume = _volume;
	
	dialTone.setVolume(volume);
	
	for (int i = 0; i < NUM_SOUND_PLAYERS; i++) {
		tonesPlayers[i].setVolume(volume);
	}
}




