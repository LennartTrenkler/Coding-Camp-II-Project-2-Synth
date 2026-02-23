#pragma once
#include "Oscillator.h"

class SawtoothOscillator : public Oscillator {
public:
    SawtoothOscillator(float freq, float sr);
    float generateSample() override;
};
