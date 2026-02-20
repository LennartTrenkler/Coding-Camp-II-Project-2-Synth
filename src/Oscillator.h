#pragma once

// Base class for all oscillators.
// It stores common properties like frequency and phase.
// Each derived oscillator defines how a sample is generated.

class Oscillator {
protected:
	float frequency; // Frequency in Hz
	float sampleRate; // Audio sample rate
	float phase; // Current phase position

public:
	Oscillator(float freq, float sr);

	// Generate one audio sample (must be implemented by child classes)
	virtual float generateSample() = 0;

	virtual ~Oscillator() = default;
};

