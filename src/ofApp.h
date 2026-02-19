#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void audioOut(ofSoundBuffer & buffer);

	ofSoundStream soundStream;
};
