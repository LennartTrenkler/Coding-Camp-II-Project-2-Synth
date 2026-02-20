#include "SquareOscillator.h"
#include "ofMain.h"
#include <cmath>

SquareOscillator::SquareOscillator(float freq, float sr)
	: Oscillator(freq, sr) {
}

float SquareOscillator::generateSample() {
	float value = sin(phase);

	// Square wave: positive = 1, negative = -1
	float sample = (value >= 0.0f) ? 1.0f : -1.0f;

	phase += TWO_PI * frequency / sampleRate;
	if (phase > TWO_PI)
		phase -= TWO_PI;

	return sample * 0.3f;
}
