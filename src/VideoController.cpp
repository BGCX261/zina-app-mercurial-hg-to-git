/*
 *  VideoController.cpp
 *  zinaApp004
 *
 *  Created by Arne Boon on 4/19/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 * NOTES
 * - movies are rewound using setPosition( 0.0f ) since neither calling
 *   firstFrame() nor setFrame( 0 ) on ofVideoPlayer seem to work
 *
 * TODO
 * - loading videos works, but handling incorrectly named / missing / extra files
 *   is untested and probably incomplete
 * - add logging on switching modes and interesting events
 */

#include "VideoController.h"
#include "zinaApp.h"


using namespace std;

const string VideoController::EXT_MOV = ".mov";
const string VideoController::THUMB_SUFFIX = "thumb";
const int VideoController::TELNUMBER_LENGTH = 10;
const int VideoController::MAX_NAME_LENGTH = 20;

const string VideoController::FONT_FILENAME = "MONACO.TTF";
const int VideoController::FONT_SIZE_PRIMARY = 40;
const int VideoController::FONT_SIZE_SIDE = 15;
const int VideoController::FONT_SIZE_FULL = VideoController::FONT_SIZE_PRIMARY;
const int VideoController::FONT_SIZE_TIME = 18;
const int VideoController::FONT_SIZE_RETURN_TEXT = 18;

const string VideoController::returnText = "Stoppen: toets #";


//-------------------------------------------------------
VideoController::VideoController()
{
	stationId = -1;
	
	videoPositions[0].set( 32, 32 ); videoSizes[0].set( 640, 360 );
	videoPositions[1].set( 704, 32 ); videoSizes[1].set( 288, 162 );
	videoPositions[2].set( 704, 219 ); videoSizes[2].set( 288, 162 );
	videoPositions[3].set( 704, 406 ); videoSizes[3].set( 288, 162 );
	primaryNamePosition.set( 20, 280 ); //relative to top-left corner of main video
	primaryNumberPosition.set( 20, 340 ); //relative to top-left corner of main video
	sideNamePosition.set( 7, 122 ); //relative to top-left corner of each side video
	sideNumberPosition.set( 7, 150 ); //relative to top-left corner of each side video
	//sideTimePosition.set(154, 288-5);
	fullNamePosition.set( 25, 504 );
	fullNumberPosition.set( 25, 564 );
	fullTimePosition.set( 870, 517);
	fullReturnPosition.set( 719, 564);
}

//-------------------------------------------------------
VideoController::~VideoController()
{}

//-------------------------------------------------------
void VideoController::setup(int _stationId)
{
	primaryFont.loadFont( FONT_FILENAME, FONT_SIZE_PRIMARY, true, true, true );
	sideFont.loadFont( FONT_FILENAME, FONT_SIZE_SIDE, true, true, true );
	fullFont.loadFont( FONT_FILENAME, FONT_SIZE_FULL, true, true, false );
	timeFont.loadFont( FONT_FILENAME, FONT_SIZE_TIME, true, true, false );
	returnFont.loadFont( FONT_FILENAME, FONT_SIZE_RETURN_TEXT, true, true, false );

	setStationID(_stationId);
	setMode( VM_PORTAL );
}

//-------------------------------------------------------
void VideoController::update()
{
	switch ( mode ) {
			
		case VM_PORTAL:
			for ( int i = 0; i < NUM_VIDEOS; i++ ) {
				if ( videos[i].thumbVideo.bLoaded ) {
					if ( ! videos[i].thumbVideo.isPlaying() ) videos[i].thumbVideo.play();
					videos[i].thumbVideo.idleMovie();
				}
			}
			break;
			
		case VM_FULL:
			if ( fullModeVideoIndex >= 0 && fullModeVideoIndex < NUM_VIDEOS ) {
				ofVideoPlayer& fvPlayer = videos[fullModeVideoIndex].fullVideo;
				
				if ( fvPlayer.bLoaded ) {
					if ( fvPlayer.isPlaying() && fvPlayer.getIsMovieDone() ) {
						setMode( VM_PORTAL );
					} else {
						if ( ! fvPlayer.isPlaying() ) {
							fvPlayer.play();
							fvPlayer.setPosition( 0.0f );
						}
						fvPlayer.idleMovie();
					}
				}
			}
			break;
			
	}
}

//-------------------------------------------------------
void VideoController::draw()
{
	ofSetColor(255, 255, 255);
	
	switch ( mode ) {
		case VM_PORTAL:
			for ( int i = 0; i < NUM_VIDEOS; i++ ) {
				if ( videos[i].thumbVideo.bLoaded ) {
					videos[i].thumbVideo.draw(videoPositions[i].x, videoPositions[i].y,
																		videoSizes[i].x, videoSizes[i].y );
				}
				
				if ( i == 0 ) {
					int numberX = videoPositions[i].x + primaryNumberPosition.x;
					int numberY = videoPositions[i].y + primaryNumberPosition.y;
					int nameX = videoPositions[i].x + primaryNamePosition.x;
					int nameY = videoPositions[i].y + primaryNamePosition.y;
					
					zinaApp::drawShadedString( primaryFont, videos[i].number, numberX, numberY, 2, 2 );
					zinaApp::drawShadedString( primaryFont, videos[i].name, nameX, nameY, 2, 2 );
				} else {
					int numberX = videoPositions[i].x + sideNumberPosition.x;
					int numberY = videoPositions[i].y + sideNumberPosition.y;
					int nameX = videoPositions[i].x + sideNamePosition.x;
					int nameY = videoPositions[i].y + sideNamePosition.y;
					
					zinaApp::drawShadedString( sideFont, videos[i].number, numberX, numberY, 1, 1 );
					zinaApp::drawShadedString( sideFont, videos[i].name, nameX, nameY, 1, 1 );
				}
			}
			break;
		case VM_FULL:
			if ( fullModeVideoIndex >= 0 && fullModeVideoIndex < NUM_VIDEOS ) {
				videoInfo& vInfo = videos[fullModeVideoIndex];
				if ( vInfo.fullVideo.bLoaded ) {
					vInfo.fullVideo.draw( 0, 0, ofGetWidth(), ofGetHeight() );
				}
				
				zinaApp::drawShadedString( fullFont, vInfo.number, fullNumberPosition.x, fullNumberPosition.y, 2, 2 );
				zinaApp::drawShadedString( fullFont, vInfo.name, fullNamePosition.x, fullNamePosition.y, 2, 2 );
				
				string trString = getTimeRemainingString( vInfo.fullVideo.getPosition(), vInfo.fullVideo.getDuration() );
				zinaApp::drawShadedString( timeFont, trString, fullTimePosition.x, fullTimePosition.y, 2, 2 );
				
				zinaApp::drawShadedString( returnFont, returnText, fullReturnPosition.x, fullReturnPosition.y, 2, 2 );
			}
			
			break;
	}
}

//-------------------------------------------------------
/*
 * Sets the video controller to a new station ID.
 * Returns true if successful or false if a problem occured.
 */
bool VideoController::setStationID(int _stationId) {
	cout << "----- Setting station ID ( " << stationId << " to " << _stationId << " )-----" << endl;
	
	//--unload movies if a station ID has already been set before
	if ( stationId > -1 ) {
		for ( int i = 0; i < NUM_VIDEOS; i++ ) {
			videos[i].thumbVideo.stop();
			videos[i].fullVideo.stop();
			videos[i].thumbVideo.closeMovie();
			videos[i].fullVideo.closeMovie();
		}
	}
	
	stationId = _stationId;
	
	cout << "-- initializing video info..." << endl;
	
	//--select main video as default
	fullModeVideoIndex = 0;
	
	//--initialize videos array with new video information and load the new movies
	bool result = initVideoInfo();
	
	if ( result ) {
		cout << "-- initialized the following video info for station ID " << stationId << endl;
		for ( int i = 0; i < NUM_VIDEOS; i++ ) {
			cout << "--  video " << i << ": {number: " << videos[i].number << ", name: " << videos[i].name << "}" << endl;
		}
	} else {
		cout << "-- failed to initialize video info for station ID " << stationId << endl;
	}
	
	return result;
}

//-------------------------------------------------------
/*
 * Checks if the requested number exists in the current video array (i.e.,
 * if a corresponding movie exists for the current station ID).
 * If so, the number is stored internally in order to show the correct movie
 * when switching to full mode.
 * Returns true if the number exists or false otherwise.
 */
bool VideoController::prepareVideoForFullMode( const string& _number ) {
	int match = -1;
	
	for ( int i = 0; i < NUM_VIDEOS; i++ ) {
		if ( videos[i].number.compare( _number ) == 0 ) {
			match = i;
			break;
		}
	}
	
	if ( match > -1 ) {
		if ( fullModeVideoIndex != match ) {
			ofVideoPlayer& prev = videos[fullModeVideoIndex].fullVideo;
			ofVideoPlayer& curr = videos[match].fullVideo;
			if ( prev.bLoaded && prev.isPlaying() ) {
				prev.setPosition( 0.0f );
				prev.stop();
			}
			if ( mode == VM_FULL && curr.bLoaded ) curr.play();
		}
		fullModeVideoIndex = match;
	}
	
	return match > -1;
}

//-------------------------------------------------------
void VideoController::setMode( VIDEO_MODE _mode ) {
	if ( mode == _mode ) return;
	
	mode = _mode;
	
	switch ( mode ) {
		case VM_PORTAL:
			if ( fullModeVideoIndex >= 0 && fullModeVideoIndex < NUM_VIDEOS ) {
				if ( videos[fullModeVideoIndex].fullVideo.bLoaded ) {
					videos[fullModeVideoIndex].fullVideo.stop();
				}
			}
			
			for ( int i = 0; i < NUM_VIDEOS; i++ ) {
				if ( videos[i].thumbVideo.bLoaded ) {
					videos[i].thumbVideo.play();
				}
			}
			
			break;
			
		case VM_FULL:
			for ( int i = 0; i < NUM_VIDEOS; i++ ) {
				if ( videos[i].thumbVideo.bLoaded ) {
					videos[i].thumbVideo.stop();
				}
			}
			
			if ( fullModeVideoIndex >= 0 && fullModeVideoIndex < NUM_VIDEOS ) {
				if ( videos[fullModeVideoIndex].fullVideo.bLoaded ) {
					videos[fullModeVideoIndex].fullVideo.play();
					videos[fullModeVideoIndex].fullVideo.setPosition( 0.0f );
				}
			}
			
			break;
		/*	
		case VM_AUDIO_RECORDING:	
			for ( int i = 0; i < NUM_VIDEOS; i++ ) {
				if ( videos[i].thumbVideo.bLoaded ) {
					videos[i].thumbVideo.stop();
				}
			}
			break;*/
	}
}

//-------------------------------------------------------
VideoController::VIDEO_MODE VideoController::getMode() {
	return mode;
}

//-------------------------------------------------------
void VideoController::setVolumeThumbs(float _volume) {
	volume = _volume;

	for (int i = 0; i < NUM_VIDEOS; i++) {
		videos[i].thumbVideo.setVolume(volume);
	}
}

//-------------------------------------------------------
void VideoController::setVolumeFull(float _volume) {
	volume = _volume;
	
	for (int i = 0; i < NUM_VIDEOS; i++) {
		videos[i].fullVideo.setVolume(volume);
	}
}

/*********************
 * PRIVATE FUNCTIONS *
 *********************/

//-------------------------------------------------------
bool VideoController::initVideoInfo()
{
	ofxDirList dirFolders;
	ofxDirList dirVideos;
	bool errorDetected = false;
	
	string stationPath = generateStationPath( stationId );
	
	//list all video directories for this station (i.e, 0, 1, 2, or 3)
	for (int i = 0; i < dirFolders.listDir(stationPath); i++) {
		string folderName = dirFolders.getName(i);
		int videoIndex = ofToInt( folderName );
		bool seenThumbFile = false;
		string* name = 0;
		string* number = 0;
		
		if ( videoIndex > NUM_VIDEOS - 1 ) {
			//LOG error "incorrect video folder name detected, highest expected number is " . NUM_VIDEOS . " (" . folderName . ")"
			errorDetected = true;
			break;
		}
		
		//list all video files inside this video dir (should just be the original and its thumbnail counterpart)
		//TODO: warn if the dir does not contain exactly 2 items?
		string videoPath = stationPath + folderName + "/";
		for (int j = 0; j < dirVideos.listDir(videoPath); j++) {
			string fileName = dirVideos.getName(j);
			int p = fileName.find(EXT_MOV);
			
			if (p != string::npos) {
				fileName.erase(p, EXT_MOV.length() );
			}
			
			vector<string> stringVector;
			zinaApp::splitString(fileName, '_', stringVector);
			
			if ( stringVector.size() == 2 || stringVector.size() == 3 ) {
				if ( stringVector.size() == 3 && stringVector[2].compare( THUMB_SUFFIX ) == 0 ) seenThumbFile = true;
				
				if ( stringVector[0].length() != TELNUMBER_LENGTH ) {
					//LOG error "filename with incorrect number encountered, should be of length " . TELNUMBER_LENGTH . " (" . path + folderName + fileName . ")"
					errorDetected = true;
					break;
				}
				
				if ( stringVector[1].length() > MAX_NAME_LENGTH ) {
					//LOG error "filename with incorrect name encountered, should be at most " . MAX_NAME_LENGTH . " characters (" . path + folderName + fileName . ")"
					errorDetected = true;
					break;
				}
				
				if ( number != 0 && name != 0 ) {
					if ( number->compare( stringVector[0] ) != 0 || name->compare( stringVector[1] ) != 0 ) {
						//LOG error "filename does not match counterpart file (" . path + folderName + fileName . ")"
						errorDetected = true;
						break;
					}
				}
				
				number = new string( stringVector[0] );
				name = new string( stringVector[1] );
			} else {
				//LOG error "incorrect filename encountered (" . path + folderName + fileName . ")"
				errorDetected = true;
				break;
			}
		}
		
		//TODO: check if seenThumbFile == true
		
		if ( ! errorDetected ) {
			videos[videoIndex].number = *number;
			videos[videoIndex].name = *name;
			videos[videoIndex].thumbVideo.loadMovie( generateVideoPath( stationId, videoIndex, *number, *name, true ) );
			videos[videoIndex].thumbVideo.setLoopState( OF_LOOP_NORMAL );
			videos[videoIndex].fullVideo.loadMovie( generateVideoPath( stationId, videoIndex, *number, *name, false ) );
			videos[videoIndex].fullVideo.setLoopState( OF_LOOP_NONE );
		}
		
		if ( number != 0 ) delete number;
		if ( name != 0 ) delete name;
		if ( errorDetected ) break;
	}
	
	return ! errorDetected;
}

string VideoController::generateStationPath( int _stationId ) {
	string path( "stations/station" );
	char idText[4] = ""; //format: nn/\0
	
	snprintf( idText, sizeof( idText ), "%2.2i/", _stationId );
	path += idText;
	
	return path;
}

//-------------------------------------------------------
string VideoController::generateVideoPath( int _stationId, int _videoNum, const string& _number, const string& _name, bool _thumb ) {
	string path = generateStationPath( _stationId );
	const int arrayLen = 5 + _number.length() + 1 + _name.length();
	char vnText[arrayLen];
	vnText[0] = '\0';
	
	snprintf( vnText, arrayLen, "%1.1i/%s_%s", _videoNum, _number.c_str(), _name.c_str() );
	path += vnText;
	
	if ( _thumb ) {
		path += "_";
		path += THUMB_SUFFIX;
	}
	path += EXT_MOV;
	
	return path;
}

//-------------------------------------------------------
string VideoController::getTimeRemainingString( float _position, float _duration ) {
	char timeText[9]; //format: (mmm:ss)\0
	int currPos = roundf( ( 1.0f - _position ) * _duration );
	int minutes = currPos / 60;
	int seconds = currPos % 60;
	
	snprintf( timeText, sizeof( timeText ), "(%.2i:%2.2i)", minutes, seconds );
	
	return string( timeText );
}
