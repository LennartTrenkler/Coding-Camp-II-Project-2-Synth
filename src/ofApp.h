#pragma once
#include "Synth.h"
#include "ofMain.h"
#include <vector>
#include <mutex>

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void audioOut(ofSoundBuffer& buffer);
    void keyPressed(int key);

private:
    Synth synth;
    ofSoundStream soundStream;
    std::vector<float> waveformBuffer;
    std::mutex bufferMutex;
    int oscillatorType = 1;
    float currentAmplitude = 0.0f;
};
