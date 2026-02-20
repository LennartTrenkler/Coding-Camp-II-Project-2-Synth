#include "ofApp.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "NoiseOscillator.h"


void ofApp::setup() {
	ofSoundStreamSettings settings;
	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;

	soundStream.setup(settings);
	soundStream.start();

	const float sr = static_cast<float>(settings.sampleRate);
	synth.setOscillator(std::make_unique<SineOscillator>(440.0f, sr));
}


void ofApp::audioOut(ofSoundBuffer & buffer) {
	for (size_t i = 0; i < buffer.getNumFrames(); i++) {
		float sample = synth.generateSample();

		for (size_t ch = 0; ch < buffer.getNumChannels(); ch++) {
			buffer[i * buffer.getNumChannels() + ch] = sample;
		}
	}
}

void ofApp::keyPressed(int key) {
	if (key == '1') {
		synth.setOscillator(
			std::make_unique<SineOscillator>(440.0f, 44100.0f));
	}

	if (key == '2') {
		synth.setOscillator(
			std::make_unique<SquareOscillator>(440.0f, 44100.0f));
	}

	if (key == '3') {
		synth.setOscillator(
			std::make_unique<NoiseOscillator>(440.0f, 44100.0f));
	}
}
