/*
 *  SampleRecorder.h
 *  ofxQTAudioSaver_Monobanda
 *
 *  Created by Arne Boon on 11/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SAMPLE_RECORDER
#define _SAMPLE_RECORDER

#include "ofMain.h"
#include "ofxSoundStream.h"
//#include "SampleStreamPlayback.h"
#include "EventArgs.h"

#include <sndfile.h>

#define SAMPLE_RATE				44100
#define BUFFER_SIZE				256
//#define RECORDING_DURATION		120
#define RECORDING_CHANNELS		2


class SampleRecorder : public ofBaseApp {

	
public:
	
	SampleRecorder();
	~SampleRecorder();
	
	//void setup(ofxSoundStream & masterMAudio, SampleStreamPlayback & sampleStreamPlayback);
	void setup(ofxSoundStream & masterMAudio, int _recordingDuration);
	void update();
	void draw();
	
	void audioInputListener (ofxAudioEventArgs &args);
	
	//void startRecording(int _stepPosition);
	void startIntroduction();
	void startRecording();
	void stopRecording();

	ofEvent<string> onLastRecordingFileName;
	
	EventArgsRecording argsRecording;
	ofEvent<EventArgsRecording>	onFinishedRecordingEvent;
	
	bool bInRecordingModus;
	
	EventArgsIntroductionRecording argsIntroductionRecording;
	ofEvent<EventArgsIntroductionRecording> onFinishedIntroductionEvent;
		
private:
	
	//-RECORDING-----------------
	//ofxSoundStream mAudio; //using masterMAudio which is passed from testApp
		
	bool bIsRecording;
	
	int recordingDuration;
	
	float * left;
	float * right;
	
	// audio buffer (device 1)
	UInt8 * mAudio_audioBuffer;
	float * mAudio_audioBufferFloat;
	int mAudio_bufferCounter;	
	
	int mAudio_playbackCounter;
	
	//bool bFinishedRecording;
	
	// device recorders
	SNDFILE * mAudio_recorder;
	
	// data
	string lastRecordingFileName;
	
	//font
	ofTrueTypeFont recordingFont;
	ofTrueTypeFont recordingCountDownFont;
	
	//SAMPLE
	ofSoundPlayer recordingIntroduction;

};

#endif	

