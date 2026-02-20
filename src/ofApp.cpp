#include "ofApp.h"
#include "SineOscillator.h"


void ofApp::setup() {

	// Configure audio stream
	ofSoundStreamSettings settings;
	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;

	soundStream.setup(settings);
	soundStream.start();

	// Create sine oscillator
	synth.setOscillator(
		std::make_unique<SineOscillator>(440.0f, 44100.0f));
}


void ofApp::audioOut(ofSoundBuffer & buffer) {
	for (size_t i = 0; i < buffer.getNumFrames(); i++) {
		float sample = synth.generateSample();

		for (size_t ch = 0; ch < buffer.getNumChannels(); ch++) {
			buffer[i * buffer.getNumChannels() + ch] = sample;
		}
	}
}
