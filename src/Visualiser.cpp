#include "Visualiser.h"

void Visualiser::setup() {
    waveformBuffer.resize(512, 0.0f);

    // Row 1 — Oscillator buttons
    sinButton      = { 120, 50,  80, 30, "Sine",     true  };
    squareButton   = { 210, 50,  80, 30, "Square",   false };
    noiseButton    = { 300, 50,  80, 30, "Noise",    false };
    sawtoothButton = { 390, 50,  80, 30, "Sawtooth", false };
    triangleButton = { 480, 50,  80, 30, "Triangle", false };

    // Row 2 — Wave speed buttons
    slowButton   = { 120, 90, 80, 30, "Slow",   false };
    mediumButton = { 210, 90, 80, 30, "Medium", true  };
    fastButton   = { 300, 90, 80, 30, "Fast",   false };

    // Row 2 — Frequency buttons
    plusButton  = { 460, 90, 40, 30, "+", false };
    minusButton = { 560, 90, 40, 30, "-", false };
}

void Visualiser::drawButton(const Button& btn) {
    if (btn.isActive) {
        ofSetColor(255);
    } else {
        ofSetColor(50);
    }
    ofDrawRectRounded(btn.x, btn.y, btn.width, btn.height, 4);

    if (btn.isActive) {
        ofSetColor(0);
    } else {
        ofSetColor(200);
    }
    ofDrawBitmapString(btn.label, btn.x + btn.width / 2 - btn.label.size() * 4, btn.y + btn.height / 2 + 5);
}

void Visualiser::draw() {
    ofBackground(0);

    // Title
    ofSetColor(255);
    ofDrawBitmapStringHighlight("Synthesizer", 40, 30, ofColor(0,0,0,0), ofColor(255));

    // Row 1 — Oscillator
    ofSetColor(150);
    ofDrawBitmapString("OSCILLATOR", 40, 47);
    drawButton(sinButton);
    drawButton(squareButton);
    drawButton(noiseButton);
    drawButton(sawtoothButton);
    drawButton(triangleButton);

    // Row 2 — Wave speed
    ofSetColor(150);
    ofDrawBitmapString("WAVE SPEED", 40, 87);
    drawButton(slowButton);
    drawButton(mediumButton);
    drawButton(fastButton);

    // Row 2 — Frequency
    ofSetColor(150);
    ofDrawBitmapString("FREQUENCY", 400, 87);
    drawButton(plusButton);
    ofSetColor(255);
    ofDrawBitmapString(ofToString((int)frequency) + " Hz", 508, 107);
    drawButton(minusButton);

    // Waveform canvas
    float canvasX = 40;
    float canvasY = 135;
    float canvasW = ofGetWidth() - 120;
    float canvasH = ofGetHeight() - 195;

    ofSetColor(30);
    ofDrawRectRounded(canvasX, canvasY, canvasW, canvasH, 8);

    // Label inside canvas
    ofSetColor(150);
    string label = "";
    if (oscillatorType == 1)      label = "SINE";
    else if (oscillatorType == 2) label = "SQUARE";
    else if (oscillatorType == 3) label = "NOISE";
    else if (oscillatorType == 4) label = "SAWTOOTH";
    else if (oscillatorType == 5) label = "TRIANGLE";
    label += "  |  " + ofToString((int)frequency) + " HZ";
    ofDrawBitmapString(label, canvasX + 16, canvasY + 24);

    // Waveform colour
    if (oscillatorType == 1)      ofSetColor(0, 200, 255);
    else if (oscillatorType == 2) ofSetColor(255, 100, 50);
    else if (oscillatorType == 3) ofSetColor(180, 100, 255);
    else if (oscillatorType == 4) ofSetColor(255, 200, 0);
    else if (oscillatorType == 5) ofSetColor(0, 255, 150);

    // Draw waveform
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        float midY = canvasY + canvasH / 2.0f;
        float scaleY = canvasH * 0.35f;
        float stepX = canvasW / (float)samplesToDisplay;

        ofSetLineWidth(2);
        ofPolyline line;
        for (int i = 0; i < samplesToDisplay && i < (int)waveformBuffer.size(); i++) {
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

void Visualiser::addSample(float sample) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    static int writePos = 0;
    waveformBuffer[writePos % waveformBuffer.size()] = sample;
    writePos++;

    float rms = 0.0f;
    for (auto s : waveformBuffer) rms += s * s;
    currentAmplitude = sqrt(rms / waveformBuffer.size());
}

int Visualiser::handleMousePressed(int x, int y) {
    int newType = -1;

    if (sinButton.contains(x, y)) {
        oscillatorType = 1;
        sinButton.isActive = true;
        squareButton.isActive = false;
        noiseButton.isActive = false;
        sawtoothButton.isActive = false;
        triangleButton.isActive = false;
        newType = 1;
    }
    if (squareButton.contains(x, y)) {
        oscillatorType = 2;
        sinButton.isActive = false;
        squareButton.isActive = true;
        noiseButton.isActive = false;
        sawtoothButton.isActive = false;
        triangleButton.isActive = false;
        newType = 2;
    }
    if (noiseButton.contains(x, y)) {
        oscillatorType = 3;
        sinButton.isActive = false;
        squareButton.isActive = false;
        noiseButton.isActive = true;
        sawtoothButton.isActive = false;
        triangleButton.isActive = false;
        newType = 3;
    }
    if (sawtoothButton.contains(x, y)) {
        oscillatorType = 4;
        sinButton.isActive = false;
        squareButton.isActive = false;
        noiseButton.isActive = false;
        sawtoothButton.isActive = true;
        triangleButton.isActive = false;
        newType = 4;
    }
    if (triangleButton.contains(x, y)) {
        oscillatorType = 5;
        sinButton.isActive = false;
        squareButton.isActive = false;
        noiseButton.isActive = false;
        sawtoothButton.isActive = false;
        triangleButton.isActive = true;
        newType = 5;
    }
    if (plusButton.contains(x, y)) {
        frequency += 10.0f;
        newType = oscillatorType;
    }
    if (minusButton.contains(x, y)) {
        frequency = max(10.0f, frequency - 10.0f);
        newType = oscillatorType;
    }
    if (slowButton.contains(x, y)) {
        samplesToDisplay = 512;
        slowButton.isActive = true;
        mediumButton.isActive = false;
        fastButton.isActive = false;
    }
    if (mediumButton.contains(x, y)) {
        samplesToDisplay = 300;
        slowButton.isActive = false;
        mediumButton.isActive = true;
        fastButton.isActive = false;
    }
    if (fastButton.contains(x, y)) {
        samplesToDisplay = 128;
        slowButton.isActive = false;
        mediumButton.isActive = false;
        fastButton.isActive = true;
    }

    return newType;
}

void Visualiser::setOscillatorType(int type) {
    oscillatorType = type;
    sinButton.isActive      = (type == 1);
    squareButton.isActive   = (type == 2);
    noiseButton.isActive    = (type == 3);
    sawtoothButton.isActive = (type == 4);
    triangleButton.isActive = (type == 5);
}
