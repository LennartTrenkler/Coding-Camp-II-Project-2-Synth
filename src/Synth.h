#pragma once
#include "Oscillator.h"
#include <memory>

// Synth acts as a simple manager.
// It owns one oscillator and asks it to generate samples.

class Synth {
private:
	std::unique_ptr<Oscillator> oscillator;

public:
	void setOscillator(std::unique_ptr<Oscillator> osc);
	float generateSample();
};

