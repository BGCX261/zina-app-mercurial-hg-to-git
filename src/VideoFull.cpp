/*
 *  VideoFull.cpp
 *  zinaApp001
 *
 *  Created by Arne Boon on 1/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VideoFull.h"

#include "ofMain.h"
#include <iostream>


//--------------------------------------------------------------
VideoFull::VideoFull()
{
	printf("---VideoFull constructor--- \n");
}

//--------------------------------------------------------------
void VideoFull::setup(){	
	bVideoLoaded = videoFull.bLoaded;
	bVideoPlaying = videoFull.isPlaying();
	bVideoMovieDone = false;
}

//--------------------------------------------------------------
void VideoFull::update() {
	
	bVideoLoaded = videoFull.bLoaded;
	bVideoPlaying = videoFull.isPlaying();
	
	if (videoFull.bLoaded) {
		bVideoMovieDone = videoFull.getIsMovieDone();
		
		videoFull.idleMovie();
	
		if( videoFull.isPlaying() && videoFull.getIsMovieDone() ) {
			videoFull.stop();
			videoFull.closeMovie();
			
			args.sLog = "VIDEO PLAYER >>> UPDATE >>> Close current video";
			ofNotifyEvent(onLogEvent, args, this);
		}
	}
}

//--------------------------------------------------------------
void VideoFull::draw() {
	
	if (videoFull.bLoaded) {
		int videoWidth = (ofGetHeight() * 640) / 480;
		videoFull.draw( (ofGetWidth()/2 ) - (videoWidth/2), 0, videoWidth, ofGetHeight());
	}
}

//--------------------------------------------------------------
void VideoFull::playVideoKeyNum(string _keypadNum) {
	int num = 0;
	
	if (_keypadNum == "*") {
		num = 10;
	} else if (_keypadNum == "#") {
		num = 11;
	} else {	
		num = ofToInt(_keypadNum);
	}
		
	//-if a video is currently playing, close it before loading a new video
	if( videoFull.isPlaying()) {
		videoFull.stop();
		videoFull.closeMovie();
				
		args.sLog = "VIDEO PLAYER >>> CLOSE KEY VIDEO >>> Close current video";
		ofNotifyEvent(onLogEvent, args, this);
	}
	
	//-load a new video
	videoFull.loadMovie( "videoKeyNum/video-" + ofToString(num) + ".mov" );
	videoFull.setLoopState(OF_LOOP_NONE);
	videoFull.play();
	
	args.sLog = "VIDEO PLAYER >>> PLAY KEY VIDEO >>> Play video NUM: " + ofToString(num);
	ofNotifyEvent(onLogEvent, args, this);
}

//--------------------------------------------------------------
void VideoFull::playVideoTelNum(string _keypadString) {
	
	string telNum = _keypadString.substr(0, 10);
		
	//-if a video is currently playing, close it before loading a new video
	if( videoFull.isPlaying()) {
		videoFull.stop();
		videoFull.closeMovie();
		
		args.sLog = "VIDEO PLAYER >>> PLAY TEL VIDEO >>> Close current video";
		ofNotifyEvent(onLogEvent, args, this);
	}
	
	//-search directory for movie files
	int numFiles = dir.listDir("videoTelNum/");
	bool bFoundNumber = false;
	
	//-break out look when found a number
	for (int i = 0; i < numFiles; i++) {
		if (dir.getName(i) == telNum + ".mov" ) {
			bFoundNumber = true;
			break;
		} else {
			bFoundNumber = false;
		}
	}
	
	if (bFoundNumber) {
		//-load a new video
		videoFull.loadMovie( "videoTelNum/" + telNum + ".mov" );
		videoFull.setLoopState(OF_LOOP_NONE);
		videoFull.play();
		
		args.sLog = "VIDEO PLAYER >>> PLAY TEL VIDEO >>> play video for: " + telNum;
		ofNotifyEvent(onLogEvent, args, this);		
	} else {
		//-TODO: number not in directory: feedback to user
		args.sLog = "VIDEO PLAYER >>> NO TEL VIDEO >>> number does not have a video: " + telNum;
		ofNotifyEvent(onLogEvent, args, this);
	}
	
}