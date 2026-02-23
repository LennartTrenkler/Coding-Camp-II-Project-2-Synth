#pragma once
#include "Synth.h"
#include "Visualiser.h"
#include "ofMain.h"

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
    Visualiser visualiser;

    void updateOscillator();
};
