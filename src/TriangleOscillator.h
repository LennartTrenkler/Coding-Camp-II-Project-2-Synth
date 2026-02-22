#pragma once
#include "Oscillator.h"

class TriangleOscillator : public Oscillator {
public:
    TriangleOscillator(float freq, float sr);
    float generateSample() override;
};
