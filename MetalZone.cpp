#include "MetalZone.h"
#include "LowPassFilter.h"

MetalZone::MetalZone() {}

MetalZone::MetalZone(float samplerate_) {
    samplerate = samplerate_;
    LowPassFilter lpf1(1001, samplerate);
    LowPassFilter lpf2(1002, samplerate);
    LowPassFilter lpf3(1003, samplerate);
    LowPassFilter lpf4(104, samplerate);
    LowPassFilter lpf5(4005, samplerate);
    LowPassFilter lpf6(6006, samplerate);
}

float MetalZone::processSample(float input, float gain) {
    float sample = input;

    sample = sample - lpf1.processSample(sample);
    sample = lpf2.processSample(sample);
    sample = lpf3.processSample(sample);

    sample = sample * gain * 200.0f;

    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }

    sample = 0.2f * sample + 0.8f * lpf4.processSample(sample);
    sample = sample - lpf5.processSample(sample) * 0.4f;
    sample = lpf6.processSample(sample);

    return sample;
}

void MetalZone::setSamplerate(float samplerate_) {
    samplerate = samplerate_;
    lpf1.setSampleRate(samplerate);
    lpf2.setSampleRate(samplerate);
    lpf3.setSampleRate(samplerate);
    lpf4.setSampleRate(samplerate);
    lpf5.setSampleRate(samplerate);
    lpf6.setSampleRate(samplerate);
}