#include "ofApp.h"

void ofApp::setup() {
	ofSoundStreamSettings settings;
	auto devices = soundStream.getDeviceList();
	for (auto & device : devices) {
		if (device.outputChannels > 0) {
			settings.setOutDevice(device);
			break;
		}
	}

	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;

	soundStream.start();
	soundStream.setup(settings);
}

void ofApp::audioOut(ofSoundBuffer & buffer) {
	static float phase = 0.0f;
	float frequency = 440.0f;
	float sampleRate = 44100.0f;

	for (size_t i = 0; i < buffer.getNumFrames(); i++) {
		float sample = sin(phase) * 0.3f;

		phase += 2.0f * PI * frequency / sampleRate;
		if (phase > 2.0f * PI)
			phase -= 2.0f * PI;

		for (size_t ch = 0; ch < buffer.getNumChannels(); ch++) {
			buffer[i * buffer.getNumChannels() + ch] = sample;
		}
	}
}
