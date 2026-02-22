#include "Visualiser.h"

void Visualiser::setup() {
    waveformBuffer.resize(512, 0.0f);

    // Row 1 — Oscillator buttons
    sinButton      = { 120, 50,  80, 30, "Sine",     true  };
    triangleButton = { 210, 50,  80, 30, "Triangle", false };
    squareButton   = { 300, 50,  80, 30, "Square",   false };
    sawtoothButton = { 390, 50,  80, 30, "Sawtooth", false };
    noiseButton    = { 480, 50,  80, 30, "Noise",    false };

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
    drawButton(triangleButton);
    drawButton(squareButton);
    drawButton(sawtoothButton);
    drawButton(noiseButton);

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
    else if (oscillatorType == 2) label = "TRIANGLE";
    else if (oscillatorType == 3) label = "SQUARE";
    else if (oscillatorType == 4) label = "SAWTOOTH";
    else if (oscillatorType == 5) label = "NOISE";
    label += "  |  " + ofToString((int)frequency) + " HZ";
    ofDrawBitmapString(label, canvasX + 16, canvasY + 24);

    // Draw waveform
    {
        float midY = canvasY + canvasH / 2.0f;
        float scaleY = canvasH * 0.35f;
        float cycles = frequency / 100.0f;

        if (oscillatorType == 1)      ofSetColor(0, 200, 255);
        else if (oscillatorType == 2) ofSetColor(0, 255, 150);
        else if (oscillatorType == 3) ofSetColor(255, 100, 50);
        else if (oscillatorType == 4) ofSetColor(255, 200, 0);
        else                          ofSetColor(180, 100, 255);

        ofSetLineWidth(2);
        ofPolyline line;

        if (oscillatorType == 5) {
            // Noise — raw buffer
            std::lock_guard<std::mutex> lock(bufferMutex);
            float stepX = canvasW / (float)samplesToDisplay;
            for (int i = 0; i < samplesToDisplay && i < (int)waveformBuffer.size(); i++) {
                line.addVertex(canvasX + i * stepX, midY - waveformBuffer[i] * scaleY);
            }
        } else {
            // Mathematical waveform with animation
            animPhase += 0.05f;

            int steps = 512;
            for (int i = 0; i < steps; i++) {
                float x = canvasX + (i / (float)steps) * canvasW;
                float phase = (i / (float)steps) * TWO_PI * cycles + animPhase;
                float sample = 0.0f;

                if (oscillatorType == 1) {
                    sample = sin(phase);
                } else if (oscillatorType == 2) {
                    float p = fmod(phase, TWO_PI);
                    if (p < PI) sample = (p / (PI / 2.0f)) - 1.0f;
                    else        sample = 3.0f - (p / (PI / 2.0f));
                } else if (oscillatorType == 3) {
                    sample = sin(phase) >= 0 ? 1.0f : -1.0f;
                } else if (oscillatorType == 4) {
                    float p = fmod(phase, TWO_PI);
                    sample = (p / PI) - 1.0f;
                }

                line.addVertex(x, midY - sample * scaleY);
            }
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
        sinButton.isActive      = true;
        triangleButton.isActive = false;
        squareButton.isActive   = false;
        sawtoothButton.isActive = false;
        noiseButton.isActive    = false;
        newType = 1;
    }
    if (triangleButton.contains(x, y)) {
        oscillatorType = 2;
        sinButton.isActive      = false;
        triangleButton.isActive = true;
        squareButton.isActive   = false;
        sawtoothButton.isActive = false;
        noiseButton.isActive    = false;
        newType = 2;
    }
    if (squareButton.contains(x, y)) {
        oscillatorType = 3;
        sinButton.isActive      = false;
        triangleButton.isActive = false;
        squareButton.isActive   = true;
        sawtoothButton.isActive = false;
        noiseButton.isActive    = false;
        newType = 3;
    }
    if (sawtoothButton.contains(x, y)) {
        oscillatorType = 4;
        sinButton.isActive      = false;
        triangleButton.isActive = false;
        squareButton.isActive   = false;
        sawtoothButton.isActive = true;
        noiseButton.isActive    = false;
        newType = 4;
    }
    if (noiseButton.contains(x, y)) {
        oscillatorType = 5;
        sinButton.isActive      = false;
        triangleButton.isActive = false;
        squareButton.isActive   = false;
        sawtoothButton.isActive = false;
        noiseButton.isActive    = true;
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
        slowButton.isActive   = true;
        mediumButton.isActive = false;
        fastButton.isActive   = false;
    }
    if (mediumButton.contains(x, y)) {
        samplesToDisplay = 300;
        slowButton.isActive   = false;
        mediumButton.isActive = true;
        fastButton.isActive   = false;
    }
    if (fastButton.contains(x, y)) {
        samplesToDisplay = 128;
        slowButton.isActive   = false;
        mediumButton.isActive = false;
        fastButton.isActive   = true;
    }

    return newType;
}

void Visualiser::setOscillatorType(int type) {
    oscillatorType = type;
    sinButton.isActive      = (type == 1);
    triangleButton.isActive = (type == 2);
    squareButton.isActive   = (type == 3);
    sawtoothButton.isActive = (type == 4);
    noiseButton.isActive    = (type == 5);
}
