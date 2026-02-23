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
    int handleMousePressed(int x, int y);
    float getFrequency() { return frequency; }
    float getVolume() { return volume; }
    int getOscillatorType() { return oscillatorType; }
    void setOscillatorType(int type);

private:
    std::vector<float> waveformBuffer;
    std::mutex bufferMutex;
    float currentAmplitude = 0.0f;
    float frequency = 440.0f;
    float volume = 0.5f;
    int oscillatorType = 1;
    int samplesToDisplay = 300;
    float animPhase = 0.0f;
    int writePos = 0;

    Button sinButton, triangleButton, squareButton, sawtoothButton, noiseButton;
    Button freqMinusButton, freqPlusButton;
    Button volMinusButton, volPlusButton;

    void drawButton(const Button& btn);
    ofColor getWaveColor();
};
