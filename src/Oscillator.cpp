#include "Oscillator.h"

Oscillator::Oscillator(float freq, float sr)
	: frequency(freq)
	, sampleRate(sr)
	, phase(0.0f) {
	// Start phase at zero
}
