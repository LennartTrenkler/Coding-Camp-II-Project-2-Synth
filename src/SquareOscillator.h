#pragma once
#include "Oscillator.h"

// Generates a square wave (hard on/off signal)

class SquareOscillator : public Oscillator {
public:
	SquareOscillator(float freq, float sr);
	float generateSample() override;
};
