#include "SineOscillator.h"
#include <cmath>
#include "ofMain.h"

SineOscillator::SineOscillator(float freq, float sr)
	: Oscillator(freq, sr) {
}

float SineOscillator::generateSample() {
	// Calculate sine of current phase
	float sample = sin(phase);

	// Advance phase
	phase += TWO_PI * frequency / sampleRate;

	// Keep phase inside 0–2PI range
	if (phase > TWO_PI)
		phase -= TWO_PI;

	// Lower volume to avoid clipping
	return sample * 0.3f;
}
