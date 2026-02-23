#include "Visualiser.h"

void Visualiser::setup() {
    waveformBuffer.resize(512, 0.0f);

    // Row 1 — Oscillator buttons (y=43)
    sinButton      = { 160, 58, 90, 30, "Sine",     true  };
    triangleButton = { 260, 58, 90, 30, "Triangle", false };
    squareButton   = { 360, 58, 90, 30, "Square",   false };
    sawtoothButton = { 460, 58, 90, 30, "Sawtooth", false };
    noiseButton    = { 560, 58, 90, 30, "Noise",    false };

    freqMinusButton = { 160, 103, 34, 30, "-", false };
    freqPlusButton  = { 280, 103, 34, 30, "+", false };

    volMinusButton = { 420, 103, 34, 30, "-", false };
    volPlusButton  = { 540, 103, 34, 30, "+", false };
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
    ofDrawBitmapString(btn.label,
        btn.x + btn.width / 2 - btn.label.size() * 4,
        btn.y + btn.height / 2 + 5);
}

ofColor Visualiser::getWaveColor() {
    float maxFreq = 2000.0f;
    float t = ofClamp(frequency / maxFreq, 0.0f, 1.0f);

    ofColor lowColor(0, 180, 255);
    ofColor midColor(180, 50, 255);
    ofColor highColor(255, 30, 30);

    if (t < 0.5f) {
        return lowColor.getLerped(midColor, t * 2.0f);
    } else {
        return midColor.getLerped(highColor, (t - 0.5f) * 2.0f);
    }
}

void Visualiser::draw() {
    ofBackground(0);
    ofSetColor(255);
    ofSetLineWidth(1);
    ofDrawBitmapString("Synthesizer", 40, 28);

    // Row 1 — Oscillator
    ofSetColor(150);
    ofDrawBitmapString("OSCILLATOR", 40, 78);
    drawButton(sinButton);
    drawButton(triangleButton);
    drawButton(squareButton);
    drawButton(sawtoothButton);
    drawButton(noiseButton);

    // Row 2 — Frequency
    ofSetColor(150);
    ofDrawBitmapString("FREQUENCY", 40, 125);
    drawButton(freqMinusButton);
    ofSetColor(255);
    float freqCenterX = freqMinusButton.x + freqMinusButton.width +
                        (freqPlusButton.x - freqMinusButton.x - freqMinusButton.width) / 2.0f;
    string freqStr = ofToString((int)frequency) + " Hz";
    ofDrawBitmapString(freqStr, freqCenterX - freqStr.size() * 4, 125);
    drawButton(freqPlusButton);

    // Row 2 — Volume
    ofSetColor(150);
    ofDrawBitmapString("VOLUME", 350, 125);
    drawButton(volMinusButton);
    ofSetColor(255);
    float volCenterX = volMinusButton.x + volMinusButton.width +
                       (volPlusButton.x - volMinusButton.x - volMinusButton.width) / 2.0f;
    string volStr = ofToString((int)(volume * 100)) + " %";
    ofDrawBitmapString(volStr, volCenterX - volStr.size() * 4, 125);
    drawButton(volPlusButton);

    // Waveform canvas
    float canvasX = 40;
    float canvasY = 153;
    float canvasW = ofGetWidth() - 120;
    float canvasH = ofGetHeight() - 213;
    
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
        float scaleY = canvasH * 0.35f * volume;
        float cycles = frequency / 100.0f;

        ofColor waveColor = getWaveColor();
        ofSetColor(waveColor);
        ofSetLineWidth(2);
        ofPolyline line;

        if (oscillatorType == 5) {
            std::lock_guard<std::mutex> lock(bufferMutex);
            float stepX = canvasW / (float)samplesToDisplay;
            for (int i = 0; i < samplesToDisplay && i < (int)waveformBuffer.size(); i++) {
                if (i < (int)waveformBuffer.size()) {
                    line.addVertex(canvasX + i * stepX, midY - (waveformBuffer[i] / 0.2f) * scaleY);
                }
            }
        } else {
            animPhase += 0.05f * (frequency / 440.0f);
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
        
        // Cursor glow on waveform
        float glowX = canvasX + canvasW / 2.0f;
        float glowPhase = (0.5f) * TWO_PI * cycles + animPhase;
        float glowSample = 0.0f;

        if (oscillatorType == 1) {
            glowSample = sin(glowPhase);
        } else if (oscillatorType == 2) {
            float p = fmod(glowPhase, TWO_PI);
            if (p < PI) glowSample = (p / (PI / 2.0f)) - 1.0f;
            else        glowSample = 3.0f - (p / (PI / 2.0f));
        } else if (oscillatorType == 3) {
            glowSample = sin(glowPhase) >= 0 ? 1.0f : -1.0f;
        } else if (oscillatorType == 4) {
            float p = fmod(glowPhase, TWO_PI);
            glowSample = (p / PI) - 1.0f;
        }

        float glowY = midY - glowSample * scaleY;
        float glowRadius = 6.0f + currentAmplitude * 20.0f;

        // Outer soft glow
        ofSetColor(waveColor.r, waveColor.g, waveColor.b, 60);
        ofDrawCircle(glowX, glowY, glowRadius * 2.5f);

        // Mid glow
        ofSetColor(waveColor.r, waveColor.g, waveColor.b, 120);
        ofDrawCircle(glowX, glowY, glowRadius);

        // Bright centre
        ofSetColor(255);
        ofDrawCircle(glowX, glowY, 3.0f);
        
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
    waveformBuffer[writePos] = sample;
    writePos = (writePos + 1) % waveformBuffer.size();

    float rms = 0.0f;
    for (auto s : waveformBuffer) rms += s * s;
    currentAmplitude = sqrt(rms / waveformBuffer.size());
}

int Visualiser::handleMousePressed(int x, int y) {
    int newType = -1;

    if (sinButton.contains(x, y)) {
        oscillatorType = 1;
        sinButton.isActive = true; triangleButton.isActive = false;
        squareButton.isActive = false; sawtoothButton.isActive = false;
        noiseButton.isActive = false;
        newType = 1;
    }
    if (triangleButton.contains(x, y)) {
        oscillatorType = 2;
        sinButton.isActive = false; triangleButton.isActive = true;
        squareButton.isActive = false; sawtoothButton.isActive = false;
        noiseButton.isActive = false;
        newType = 2;
    }
    if (squareButton.contains(x, y)) {
        oscillatorType = 3;
        sinButton.isActive = false; triangleButton.isActive = false;
        squareButton.isActive = true; sawtoothButton.isActive = false;
        noiseButton.isActive = false;
        newType = 3;
    }
    if (sawtoothButton.contains(x, y)) {
        oscillatorType = 4;
        sinButton.isActive = false; triangleButton.isActive = false;
        squareButton.isActive = false; sawtoothButton.isActive = true;
        noiseButton.isActive = false;
        newType = 4;
    }
    if (noiseButton.contains(x, y)) {
        oscillatorType = 5;
        sinButton.isActive = false; triangleButton.isActive = false;
        squareButton.isActive = false; sawtoothButton.isActive = false;
        noiseButton.isActive = true;
        newType = 5;
    }
    if (freqMinusButton.contains(x, y)) {
        frequency = max(10.0f, frequency - 10.0f);
        newType = oscillatorType;
    }
    if (freqPlusButton.contains(x, y)) {
        frequency += 10.0f;
        newType = oscillatorType;
    }
    if (volMinusButton.contains(x, y)) {
        volume = std::max(0.0f, volume - 0.05f);
    }
    if (volPlusButton.contains(x, y)) {
        volume = std::min(1.0f, volume + 0.05f);
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
