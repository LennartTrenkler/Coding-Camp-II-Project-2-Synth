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

    // Define buttons
    sinButton    = { 40,  80, 140, 44, "Sine",   true  };
    squareButton = { 200, 80, 140, 44, "Square", false };
    noiseButton  = { 360, 80, 140, 44, "Noise",  false };
}

void ofApp::update() {
}

void ofApp::drawButton(const Button& btn) {
    if (btn.isActive) {
        ofSetColor(255); // white fill when active
    } else {
        ofSetColor(50);  // dark fill when inactive
    }
    ofDrawRectRounded(btn.x, btn.y, btn.width, btn.height, 6);

    // Label
    if (btn.isActive) {
        ofSetColor(0);   // black text on white
    } else {
        ofSetColor(200); // light text on dark
    }
    ofDrawBitmapString(btn.label, btn.x + btn.width / 2 - btn.label.size() * 4, btn.y + btn.height / 2 + 5);
}

void ofApp::draw() {
    ofBackground(0);

    // Title
    ofSetColor(255);
    ofDrawBitmapStringHighlight("Synthesizer", 40, 40, ofColor(0,0,0,0), ofColor(255));

    // Buttons
    drawButton(sinButton);
    drawButton(squareButton);
    drawButton(noiseButton);

    // Waveform canvas
    float canvasX = 40;
    float canvasY = 160;
    float canvasW = ofGetWidth() - 120;
    float canvasH = ofGetHeight() - 220;

    ofSetColor(30);
    ofDrawRectRounded(canvasX, canvasY, canvasW, canvasH, 8);

    // Oscillator label inside canvas
    ofSetColor(150);
    string label = "";
    if (oscillatorType == 1) label = "SINE";
    else if (oscillatorType == 2) label = "SQUARE";
    else label = "NOISE";
    label += "  |  440 HZ";
    ofDrawBitmapString(label, canvasX + 16, canvasY + 24);

    // Waveform colour
    if (oscillatorType == 1) ofSetColor(0, 200, 255);
    else if (oscillatorType == 2) ofSetColor(255, 100, 50);
    else ofSetColor(180, 100, 255);

    // Draw waveform
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        float midY = canvasY + canvasH / 2.0f;
        float scaleY = canvasH * 0.35f;
        int samplesToDisplay = 256;
        float stepX = canvasW / (float)samplesToDisplay;

        ofSetLineWidth(2);
        ofPolyline line;
        for (int i = 0; i < samplesToDisplay; i++) {
            line.addVertex(canvasX + i * stepX, midY - waveformBuffer[i] * scaleY);
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
    ofDrawBitmapString("VOL", barX - 2, ofGetHeight() - 20.0f);
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

    float rms = 0.0f;
    for (auto s : waveformBuffer) rms += s * s;
    currentAmplitude = sqrt(rms / waveformBuffer.size());
}

void ofApp::mousePressed(int x, int y, int button) {
    if (sinButton.contains(x, y)) {
        oscillatorType = 1;
        sinButton.isActive = true;
        squareButton.isActive = false;
        noiseButton.isActive = false;
        synth.setOscillator(std::make_unique<SineOscillator>(440.0f, 44100.0f));
    }
    if (squareButton.contains(x, y)) {
        oscillatorType = 2;
        sinButton.isActive = false;
        squareButton.isActive = true;
        noiseButton.isActive = false;
        synth.setOscillator(std::make_unique<SquareOscillator>(440.0f, 44100.0f));
    }
    if (noiseButton.contains(x, y)) {
        oscillatorType = 3;
        sinButton.isActive = false;
        squareButton.isActive = false;
        noiseButton.isActive = true;
        synth.setOscillator(std::make_unique<NoiseOscillator>(440.0f, 44100.0f));
    }
}

void ofApp::keyPressed(int key) {
    if (key == '1') {
        oscillatorType = 1;
        sinButton.isActive = true;
        squareButton.isActive = false;
        noiseButton.isActive = false;
        synth.setOscillator(std::make_unique<SineOscillator>(440.0f, 44100.0f));
    }
    if (key == '2') {
        oscillatorType = 2;
        sinButton.isActive = false;
        squareButton.isActive = true;
        noiseButton.isActive = false;
        synth.setOscillator(std::make_unique<SquareOscillator>(440.0f, 44100.0f));
    }
    if (key == '3') {
        oscillatorType = 3;
        sinButton.isActive = false;
        squareButton.isActive = false;
        noiseButton.isActive = true;
        synth.setOscillator(std::make_unique<NoiseOscillator>(440.0f, 44100.0f));
    }
}
