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
    // Background tinted by amplitude
    int brightness = currentAmplitude * 60;
    ofBackground(0, brightness / 4, brightness / 2);

    // Soft glowing circle in centre
    ofSetColor(50, 100, 200, currentAmplitude * 200);
    ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, currentAmplitude * 300);

    // Key hint
    ofSetColor(255);
    ofDrawBitmapString("Press 1 (Sine)  2 (Square)  3 (Noise)", 20, 20);

    // Oscillator label and frequency
    string label = "Oscillator: ";
    if (oscillatorType == 1) label += "Sine";
    else if (oscillatorType == 2) label += "Square";
    else label += "Noise";
    label += "  |  440 Hz";
    ofDrawBitmapString(label, 20, ofGetHeight() - 20);

    // Waveform colour based on oscillator type
    if (oscillatorType == 1) ofSetColor(0, 200, 255);
    else if (oscillatorType == 2) ofSetColor(255, 100, 50);
    else ofSetColor(180, 100, 255);

    // Waveform
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        float midY = ofGetHeight() / 2.0f;
        float scaleY = 200.0f;
        float stepX = (float)ofGetWidth() / waveformBuffer.size();

        ofSetLineWidth(2);
        ofPolyline line;
        for (int i = 0; i < (int)waveformBuffer.size(); i++) {
            line.addVertex(i * stepX, midY - waveformBuffer[i] * scaleY);
        }
        line.draw();
    }

    // Amplitude bar
    float margin = 20.0f;
    float barWidth = 20.0f;
    float maxBarHeight = ofGetHeight() - 60.0f;
    float barHeight = currentAmplitude * maxBarHeight;
    float barX = ofGetWidth() - barWidth - margin;
    float barY = ofGetHeight() - barHeight - 40.0f;

    ofSetColor(255);
    ofDrawRectangle(barX, barY, barWidth, barHeight);
    ofDrawBitmapString("Vol", barX, ofGetHeight() - 20.0f);
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

    // Compute RMS amplitude
    float rms = 0.0f;
    for (auto s : waveformBuffer) rms += s * s;
    currentAmplitude = sqrt(rms / waveformBuffer.size());
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
