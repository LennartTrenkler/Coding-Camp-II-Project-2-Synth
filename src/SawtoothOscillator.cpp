#include "SawtoothOscillator.h"
#include "ofMain.h"

SawtoothOscillator::SawtoothOscillator(float freq, float sr)
    : Oscillator(freq, sr) {
}

float SawtoothOscillator::generateSample() {
    // Ramps from -1 to 1 over one cycle
    float sample = (phase / PI) - 1.0f;

    phase += TWO_PI * frequency / sampleRate;
    if (phase > TWO_PI)
        phase -= TWO_PI;

    return sample * 0.3f;
}
