#pragma once
#include "ofMain.h"
#include <vector>
#include <mutex>

struct Button {
    float x, y, width, height;
    string label;
    bool isActive;

    bool contains(float mx, float my) {
        return mx >= x && mx <= x + width && my >= y && my <= y + height;
    }
};

class Visualiser {
public:
    void setup();
    void draw();
    void addSample(float sample);

    // Button interaction — returns oscillator type if changed, -1 if not
    int handleMousePressed(int x, int y);

    // Frequency
    float getFrequency() { return frequency; }

    // Let ofApp read oscillator type
    int getOscillatorType() { return oscillatorType; }

    // Let ofApp set oscillator type (for keyboard shortcuts)
    void setOscillatorType(int type);

private:
    std::vector<float> waveformBuffer;
    std::mutex bufferMutex;
    float currentAmplitude = 0.0f;
    float frequency = 440.0f;
    int oscillatorType = 1;
    int samplesToDisplay = 300;

    Button sinButton, squareButton, noiseButton;
    Button plusButton, minusButton;
    Button slowButton, mediumButton, fastButton;

    void drawButton(const Button& btn);
    void updateAmplitude();
};
