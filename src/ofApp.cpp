#include "ofApp.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "NoiseOscillator.h"
#include "SawtoothOscillator.h"
#include "TriangleOscillator.h"

void ofApp::setup() {
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = 44100;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = 512;

    soundStream.setup(settings);
    soundStream.start();

    synth.setOscillator(std::make_unique<SineOscillator>(440.0f, 44100.0f));
    visualiser.setup();
}

void ofApp::update() {
}

void ofApp::draw() {
    visualiser.draw();
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
        float sample = synth.generateSample();
        visualiser.addSample(sample);

        for (size_t ch = 0; ch < buffer.getNumChannels(); ch++) {
            buffer[i * buffer.getNumChannels() + ch] = sample;
        }
    }
}

void ofApp::updateOscillator() {
    int type = visualiser.getOscillatorType();
    float freq = visualiser.getFrequency();

    if (type == 1)
        synth.setOscillator(std::make_unique<SineOscillator>(freq, 44100.0f));
    else if (type == 2)
        synth.setOscillator(std::make_unique<TriangleOscillator>(freq, 44100.0f));
    else if (type == 3)
        synth.setOscillator(std::make_unique<SquareOscillator>(freq, 44100.0f));
    else if (type == 4)
        synth.setOscillator(std::make_unique<SawtoothOscillator>(freq, 44100.0f));
    else if (type == 5)
        synth.setOscillator(std::make_unique<NoiseOscillator>(freq, 44100.0f));
}

void ofApp::mousePressed(int x, int y, int button) {
    int result = visualiser.handleMousePressed(x, y);
    if (result != -1) {
        updateOscillator();
    }
}

void ofApp::keyPressed(int key) {
    if (key == '1') { visualiser.setOscillatorType(1); updateOscillator(); }
    if (key == '2') { visualiser.setOscillatorType(2); updateOscillator(); }
    if (key == '3') { visualiser.setOscillatorType(3); updateOscillator(); }
    if (key == '4') { visualiser.setOscillatorType(4); updateOscillator(); }
    if (key == '5') { visualiser.setOscillatorType(5); updateOscillator(); }
}
