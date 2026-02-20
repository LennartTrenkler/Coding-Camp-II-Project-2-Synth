#include "Synth.h"

void Synth::setOscillator(std::unique_ptr<Oscillator> osc) {
	// Replace current oscillator
	oscillator = std::move(osc);
}

float Synth::generateSample() {
	if (oscillator)
		return oscillator->generateSample();

	return 0.0f; // Silence if no oscillator set
}
