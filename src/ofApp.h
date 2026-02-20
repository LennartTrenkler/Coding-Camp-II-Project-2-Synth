#pragma once
#include "Synth.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void audioOut(ofSoundBuffer & buffer);

private:
	Synth synth;
	ofSoundStream soundStream;
};
