#include "TriangleOscillator.h"
#include "ofMain.h"

TriangleOscillator::TriangleOscillator(float freq, float sr)
    : Oscillator(freq, sr) {
}

float TriangleOscillator::generateSample() {
    // Triangle wave: rises from -1 to 1 then falls back
    float sample;
    if (phase < PI) {
        sample = (phase / (PI / 2.0f)) - 1.0f;
    } else {
        sample = 3.0f - (phase / (PI / 2.0f));
    }

    phase += TWO_PI * frequency / sampleRate;
    if (phase > TWO_PI)
        phase -= TWO_PI;

    return sample * 0.3f;
}
