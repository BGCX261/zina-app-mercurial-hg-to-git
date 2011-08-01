/*
 *  AudioComposition.cpp
 *  zinaApp
 *
 *  Created by Arne Boon on 4/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioComposition.h"

#include "ofMain.h"
#include <iostream>


//--------------------------------------------------------------
AudioComposition::AudioComposition()
{
	printf("---AudioComposition constructor--- \n");
}

//--------------------------------------------------------------
AudioComposition::~AudioComposition()
{
	//
}

//--------------------------------------------------------------
void AudioComposition::setup(int _stationId){
	
	stationId = _stationId;
	
	if (track.bLoadedOk == true) {
		track.unloadSound();
	}
	
	track.loadSoundWithTarget("sound/audioComposition/" + ofToString(stationId) + "_audioFile.aif", 1);
	track.setMultiPlay(false);
	track.setLoop(true);

}

//--------------------------------------------------------------
void AudioComposition::update() {
	ofSoundUpdate();
}

//--------------------------------------------------------------
void AudioComposition::play() {
	track.play();
}

//--------------------------------------------------------------
void AudioComposition::stop() {
	track.stop();
}

//--------------------------------------------------------------
void AudioComposition::setVolume(float _volume) {
	volume = _volume;
	
	track.setVolume(volume);
}
