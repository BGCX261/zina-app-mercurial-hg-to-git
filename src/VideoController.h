/*
 *  VideoController.h
 *  zinaApp004
 *
 *  Created by Arne Boon on 4/19/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIDEO_CONTROLLER
#define _VIDEO_CONTROLLER

#include <string>
#include "ofMain.h"
#include "ofEvents.h"
#include "EventArgs.h"

#include "ofxDirList.h"

#define NUM_VIDEOS 4

class VideoController : ofBaseApp {
public:
	enum VIDEO_MODE {
		VM_PORTAL,
		VM_FULL,
		//VM_AUDIO_RECORDING
	};
	
	struct videoInfo {
		string name;
		string number;
		ofVideoPlayer thumbVideo;
		ofVideoPlayer fullVideo;
	};
	
	static const string EXT_MOV;
	static const string THUMB_SUFFIX;
	static const int TELNUMBER_LENGTH;
	static const int MAX_NAME_LENGTH;
	
	static const string FONT_FILENAME;
	static const int FONT_SIZE_PRIMARY;
	static const int FONT_SIZE_SIDE;
	static const int FONT_SIZE_FULL;
	static const int FONT_SIZE_TIME;
	static const int FONT_SIZE_RETURN_TEXT;
	
	VideoController();
	~VideoController();
	
	void setup(int stationId);
	void update();
	void draw();
	
	void play();
	void stop();
	
	bool setStationID(int _stationId);
	bool prepareVideoForFullMode( const string& _number );
	void setMode( VIDEO_MODE _mode );
	
	void setVolumeThumbs( float volume );
	void setVolumeFull( float volume );
	
	VIDEO_MODE getMode();
	
private:
	ofPoint videoPositions[NUM_VIDEOS], videoSizes[NUM_VIDEOS];
	ofPoint primaryNamePosition, primaryNumberPosition; //relative to top-left corner of main video
	ofPoint sideNamePosition, sideNumberPosition;				//relative to top-left corner of each side video
	ofPoint fullNamePosition, fullNumberPosition, fullTimePosition, fullReturnPosition;
	
	static const string returnText;
	
	videoInfo videos[NUM_VIDEOS];
	ofTrueTypeFont primaryFont, sideFont, fullFont, timeFont, returnFont;
	
	int stationId;
	VIDEO_MODE mode;
	int fullModeVideoIndex;
	
	float volume;
	
	EventArgsLogger logArgs;
	ofEvent<EventArgsLogger> onLogEvent;
	
	void load();
	bool initVideoInfo();
	string generateStationPath( int _stationId );
	string generateVideoPath( int _stationId, int _videoNum, const string& _number, const string& _name, bool _thumb );
	string getTimeRemainingString( float _position, float _duration );
};

#endif