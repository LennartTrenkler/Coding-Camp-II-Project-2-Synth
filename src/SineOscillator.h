#pragma once
#include "Oscillator.h"

// Generates a sine wave signal.

class SineOscillator : public Oscillator {
public:
	SineOscillator(float freq, float sr);

	float generateSample() override;
};

