#pragma once
#include "Oscillator.h"

class NoiseOscillator : public Oscillator {
public:
	NoiseOscillator(float freq, float sr);
	float generateSample() override;
};
