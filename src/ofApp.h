#pragma once
#include "Synth.h"
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

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void audioOut(ofSoundBuffer& buffer);
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

private:
    Synth synth;
    ofSoundStream soundStream;

    std::vector<float> waveformBuffer;
    std::mutex bufferMutex;
    int oscillatorType = 1;
    float currentAmplitude = 0.0f;

    Button sinButton, squareButton, noiseButton;
    void drawButton(const Button& btn);
};
