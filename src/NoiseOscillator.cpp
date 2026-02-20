#include "NoiseOscillator.h"
#include "ofMain.h"

NoiseOscillator::NoiseOscillator(float freq, float sr)
	: Oscillator(freq, sr) {
}

float NoiseOscillator::generateSample() {
	// White noise ignores frequency
	return ofRandom(-1.0f, 1.0f) * 0.3f;
}
