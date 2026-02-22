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

    waveformBuffer.resize(512, 0.0f);
}

void ofApp::update() {
    // placeholder for now
}

void ofApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("Press 1 (Sine)  2 (Square)  3 (Noise)", 20, 20);
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
    std::lock_guard<std::mutex> lock(bufferMutex);

    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
        float sample = synth.generateSample();

        waveformBuffer[i % waveformBuffer.size()] = sample;

        for (size_t ch = 0; ch < buffer.getNumChannels(); ch++) {
            buffer[i * buffer.getNumChannels() + ch] = sample;
        }
    }
}

void ofApp::keyPressed(int key) {
    if (key == '1') {
        oscillatorType = 1;
        synth.setOscillator(std::make_unique<SineOscillator>(440.0f, 44100.0f));
    }
    if (key == '2') {
        oscillatorType = 2;
        synth.setOscillator(std::make_unique<SquareOscillator>(440.0f, 44100.0f));
    }
    if (key == '3') {
        oscillatorType = 3;
        synth.setOscillator(std::make_unique<NoiseOscillator>(440.0f, 44100.0f));
    }
}
