/*
 *  SampleRecorder.cpp
 *  ofxQTAudioSaver_Monobanda
 *
 *  Created by Arne Boon on 11/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SampleRecorder.h"
#include "stdio.h"

//-------------------------------------------------------------
SampleRecorder::SampleRecorder(){

}

//-------------------------------------------------------------
SampleRecorder::~SampleRecorder(){
	if (bIsRecording) stopRecording();
	
	//--NOTICE: commented out to prevent crash
	//ofSoundStreamStop();
	//ofSoundStreamClose();
	
	// clear memory
	delete [] left;
	delete [] right;
	
	delete [] mAudio_audioBuffer;
	delete [] mAudio_audioBufferFloat;
		
	cout << "SAMPLE RECORDER: clear memory" << endl;
}

//--------------------------------------------------------------
//void SampleRecorder::setup(ofxSoundStream & masterMAudio, SampleStreamPlayback & sampleStreamPlayback){	
void SampleRecorder::setup(ofxSoundStream & masterMAudio, int _recordingDuration){	
	
	recordingDuration = _recordingDuration;
	volumeRecIntro = 0.0;			//is set through function setVolumeRecIntro() via gui
	
	left = new float[BUFFER_SIZE];
	right = new float[BUFFER_SIZE];
	
	for (int i = 0; i < BUFFER_SIZE; i++) {
		left[i] = 0;
		right[i] = 0;
	}
	
	// create an empty buffer for device 1 (2 channels)
	int mAudio_audioBufferInt = SAMPLE_RATE * recordingDuration * RECORDING_CHANNELS;
	mAudio_audioBuffer = new UInt8[mAudio_audioBufferInt];
	mAudio_audioBufferFloat = new float[mAudio_audioBufferInt];
	
	mAudio_bufferCounter = 0;
	
	bIsRecording = false;
	bInRecordingModus = false;
	
	ofAddListener(masterMAudio.audioReceivedEvent, this, &SampleRecorder::audioInputListener);
	//ofAddListener(sampleStreamPlayback.onNextStepEvent, this, &SampleRecorder::startRecording);
	
	//--font
	recordingFont.loadFont("MONACO.TTF", 16, true, true, false);
	recordingCountDownFont.loadFont("MONACO.TTF", 120, true, true, false);
	
	//--sound
	recordingIntroduction.setMultiPlay(false);
	recordingIntroduction.setLoop(false);
	recordingIntroduction.loadSound("sound/recIntro/recIntro.wav", false);
}

//--------------------------------------------------------------
void SampleRecorder::update(){
	
	if (recordingIntroduction.bLoadedOk) {
		if ( recordingIntroduction.getIsPlaying() ) {			
			if (recordingIntroduction.getPosition() > 0.96) {
			
				recordingIntroduction.stop();
				recordingIntroduction.setPosition(0.0);
				
				cout << "SampleRecorder::update() introduction fineshed, notify to listener zinaApp to call startRecording()" << endl;
				
				argsIntroductionRecording.bFinishedIntroduction = true;
				ofNotifyEvent(onFinishedIntroductionEvent, argsIntroductionRecording, this);
				
				
			}
		}
	}
	
}

//--------------------------------------------------------------
void SampleRecorder::draw(){
	
	ofSetColor(255,255,255);
	recordingFont.drawString("Na de inleidende stem vanuit de telefoonhoorn, ", 100, 100);
	recordingFont.drawString("heeft u " + ofToString( recordingDuration ) + " seconden om een reactie in te spreken" , 100, 130);
	recordingFont.drawString("Stoppen: toets #" , 100, 500);
	
	//--draw the count down
	float currentTime = recordingDuration - (mAudio_bufferCounter * recordingDuration) / ((SAMPLE_RATE * recordingDuration) / BUFFER_SIZE);
	recordingCountDownFont.drawString( ofToString(currentTime, 0) , 520, 350);
	
	// draw the left:
	ofSetColor(100,100,100);
	int waveX = 105;
	int waveY = 200;
	int waveW = 256;
	int waveH = 200;
	ofRect(waveX,waveY,waveW,waveH);
	ofSetColor(255,255,255);
	for (int i = 0; i < waveW; i++){
		ofLine(waveX+i, waveY + waveH/2, waveX+i, waveY + waveH/2 + left[i]*150.0);
	}
	
	/*/ draw the right:
	ofSetColor(100,100,100);
	ofRect(400,100,256,200);
	ofSetColor(255,255,255);
	for (int i = 0; i < 256; i++){
		ofLine(400+i,200,400+i,200+right[i]*100.0f);
	}
	*/
	
	// draw the recording
	/*
	int stepsSamples = 60;
	ofSetColor(100,100,100);
	ofRect(100, 340, 44100/stepsSamples,200);
	for (int i = 0; i < recordingDuration * SAMPLE_RATE * RECORDING_CHANNELS; i+=stepsSamples){
		
		float xPos = 100+i/(recordingDuration * RECORDING_CHANNELS * stepsSamples);
		float offset = mAudio_audioBufferFloat[i]*100;
		
		ofSetColor(255,100,100);
		ofLine(xPos,440,xPos,440+offset);
	}
	*/
	
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void SampleRecorder::startIntroduction() {
	
	bInRecordingModus = true;
	recordingIntroduction.play();
	
	cout << "playing introduction" << endl;
	
}

//-START - STOP RECORDING-------------------------------------------------------------
//--------------------------------------------------------------
//void SampleRecorder::startRecording(int _stepPosition){
void SampleRecorder::startRecording(){
			
	//if(!bIsRecording && _stepPosition == 2){
	if(!bIsRecording){
		
		//audio info
		SF_INFO mAudio_info;
		
		//--clear buffers
		for (int i = 0; i < BUFFER_SIZE; i++) {
			left[i] = 0;
			right[i] = 0;
		}
		
		for (int i = 0; i < recordingDuration * SAMPLE_RATE * RECORDING_CHANNELS; i++) {
			mAudio_audioBufferFloat[i] = 0;
			mAudio_audioBufferFloat[i] = 0;
		}
		
		mAudio_info.frames = SAMPLE_RATE * 60;
		mAudio_info.samplerate = SAMPLE_RATE;
		mAudio_info.channels = RECORDING_CHANNELS;
		mAudio_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
		
		lastRecordingFileName = "recordings/" 
								+ ofToString( ofGetYear() ) + "_"
								+ ofToString( ofGetMonth() ) + "_"
								+ ofToString( ofGetDay() ) + "_"
								+ ofToString( ofGetHours() ) + "_"
								+ ofToString( ofGetMinutes() ) + "_"
								+ ofToString( ofGetSeconds() ) +
								+ ".wav";
		
		ofNotifyEvent(onLastRecordingFileName, lastRecordingFileName, this);
		
		mAudio_recorder = sf_open(ofToDataPath(lastRecordingFileName).c_str(), SFM_WRITE, &mAudio_info);
		
		mAudio_bufferCounter = 0;
		bIsRecording = true;

		cout << "SAMPLE RECORDER >> START RECORDING: " << lastRecordingFileName << endl;
	}
}

//--------------------------------------------------------------
void SampleRecorder::stopRecording(){
	
	//private (for actuall recording of data)
	if(bIsRecording){
		bIsRecording = false;
		sf_close(mAudio_recorder);
				
		cout << "SAMPLE RECORDER >> STOP RECORDING: " << lastRecordingFileName << endl;
	}
	
	//--clear buffers
	for (int i = 0; i < BUFFER_SIZE; i++) {
		left[i] = 0;
		right[i] = 0;
	}
	
	//introduction text
	recordingIntroduction.stop();
	recordingIntroduction.setPosition(0.0);
	
	//public (some if's in zinaApp count on this one)
	bInRecordingModus = false;
	
	//notify finished (listener in zinaApp for regulating volume of vids)
	argsRecording.bIsRecording = false;
	ofNotifyEvent(onFinishedRecordingEvent, argsRecording, this);

}

//--------------------------------------------------------------
void SampleRecorder::audioInputListener(ofxAudioEventArgs &args){	
			
	if(bIsRecording){
		
		int maxBuffers = SAMPLE_RATE * recordingDuration / args.bufferSize;
			
		//if(args.deviceName == "M-Audio, Inc.: M-Audio Fast Track Pro USB"){
			
			//from stereo to mono over 2 channels
			for (int i = 0; i < args.bufferSize; i++) {
				args.buffer[i*2+1] = args.buffer[i*2];
			}
						
			if(mAudio_bufferCounter < maxBuffers){
				for (int i = 0; i < args.bufferSize; i++){
					left[i] = args.buffer[i*2];
					right[i] = args.buffer[i*2+1];
										
					// interleave samples (change 2 integer) [[no need to fill since only mAudio_audioBufferFloat is used lateron]]
					//mAudio_audioBuffer[mAudio_bufferCounter * args.bufferSize * args.nChannels+i*args.nChannels] =  args.buffer[i*args.nChannels] * 256;
					//mAudio_audioBuffer[mAudio_bufferCounter * args.bufferSize * args.nChannels+i*args.nChannels+1] =  args.buffer[i*args.nChannels+1] * 256;
					
					mAudio_audioBufferFloat[mAudio_bufferCounter * args.bufferSize * args.nChannels+i*args.nChannels] =  args.buffer[i*args.nChannels];
					mAudio_audioBufferFloat[mAudio_bufferCounter * args.bufferSize * args.nChannels+i*args.nChannels+1] =  args.buffer[i*args.nChannels+1];
				}
			
				sf_write_float(mAudio_recorder, args.buffer, args.bufferSize*2);
				
				mAudio_bufferCounter++;
			}
		//}
		
		if((mAudio_bufferCounter == maxBuffers)){
			this->stopRecording();
		}
		
	}
} 

//--------------------------------------------------------------
void SampleRecorder::setVolumeRecIntro(float _volume) {
	volumeRecIntro = _volume;
	
	recordingIntroduction.setVolume(volumeRecIntro);
}