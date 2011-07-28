/*
 *  VideoPortal.cpp
 *  zinaApp003
 *
 *  Created by Arne Boon on 3/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VideoPortal.h"

#include "ofMain.h"
#include <iostream>


//--------------------------------------------------------------
VideoPortal::VideoPortal()
{
	printf("---VideoPortal constructor--- \n");
}

//--------------------------------------------------------------
//- SETUP ------------------------------------------------------
//--------------------------------------------------------------
void VideoPortal::setup(int _id){	
	bVideoLoaded = videoPortal.bLoaded;
	bVideoPlaying = videoPortal.isPlaying();
	
	videoPortalId = _id;
	
	
	yTemp, offset = 0;
	
	cout << "videoPortalId = " << videoPortalId << endl;
}

//--------------------------------------------------------------
//- UPDATE -----------------------------------------------------
//--------------------------------------------------------------
void VideoPortal::update() {
	
	bVideoLoaded = videoPortal.bLoaded;
	bVideoPlaying = videoPortal.isPlaying();
	
	if (videoPortal.bLoaded) {	
		videoPortal.idleMovie();
	}
}

//--------------------------------------------------------------
//- DRAW -------------------------------------------------------
//--------------------------------------------------------------
void VideoPortal::draw() {
	
	if (videoPortal.bLoaded) {
		int videoWidth = (ofGetHeight() * 640) / 480;
		videoPortal.draw(x, y, w, h);
	}
}

//--------------------------------------------------------------
//- LOAD -------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void VideoPortal::load(string _filename) {
	
	//-if a video is currently playing, close it before loading a new video
	if( videoPortal.isPlaying()) {
		videoPortal.stop();
		videoPortal.closeMovie();
		
		args.sLog = "VIDEO PLAYER >>> PLAY TEL VIDEO >>> Close current video";
		ofNotifyEvent(onLogEvent, args, this);
	}
	
	//-search directory for movie files
	int numFiles = dir.listDir("videoPortal/");
	bool bFoundNumber = false;
	
	//-break out look when found a number
	for (int i = 0; i < numFiles; i++) {
		if (dir.getName(i) == _filename) {
			bFoundNumber = true;
			break;
		} else {
			bFoundNumber = false;
		}
	}
	
	if (bFoundNumber) {
		//-load a new video
		videoPortal.loadMovie( "videoPortal/" + _filename );
		
		args.sLog = "VIDEO PLAYER >>> PLAY TEL VIDEO >>> play video for: " + _filename;
		ofNotifyEvent(onLogEvent, args, this);		
	} else {
		//-TODO: number not in directory: feedback to user
		args.sLog = "VIDEO PLAYER >>> NO TEL VIDEO >>> number does not have a video: " + _filename;
		ofNotifyEvent(onLogEvent, args, this);
	}
}

//--------------------------------------------------------------
void VideoPortal::close() {
	if(videoPortal.bLoaded) {
		videoPortal.close();
	}	
}

//--------------------------------------------------------------
void VideoPortal::play() {
	if(videoPortal.bLoaded) {
		videoPortal.setLoopState(OF_LOOP_NORMAL);
		videoPortal.play();
	}	
}

//--------------------------------------------------------------
void VideoPortal::stop() {
	if (videoPortal.isPlaying()) {
		videoPortal.stop();
	}
}


//--------------------------------------------------------------
//- SETTERS ----------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
void VideoPortal::setX(int _x) {
	x = _x;
}

//--------------------------------------------------------------
void VideoPortal::setY(int _y) {
	
	y = _y;

	yTemp = _y;
	updatePosition();
		
}

//--------------------------------------------------------------
void VideoPortal::setSize(int _size) {

	w = 4 * _size;
	h = 3 * _size;
	
	updatePosition();
}

//--------------------------------------------------------------
void VideoPortal::setOffset(int _offset) {
	
	offset = _offset;
	
	updatePosition();
	
}

//--------------------------------------------------------------
void VideoPortal::updatePosition() {
	
	if (videoPortalId != 0) {
		y = yTemp + ( (h + offset) * videoPortalId);
	}
	
}

//--------------------------------------------------------------
//- GETTERS ----------------------------------------------------
//--------------------------------------------------------------


//--------------------------------------------------------------
int VideoPortal::getX() {
	
	return x;
}

//--------------------------------------------------------------
int VideoPortal::getY() {
	
	return y;
}

//--------------------------------------------------------------
int VideoPortal::getHeight() {
	
	return h;
}

//--------------------------------------------------------------
int VideoPortal::getWidth() {
	
	return w;
}





