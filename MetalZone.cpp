#include "MetalZone.h"
#include "LowPassFilter.h"

MetalZone::MetalZone() {}

MetalZone::MetalZone(float samplerate_) {
    samplerate = samplerate_;

    lpf1.setSampleRate(samplerate);
    lpf2.setSampleRate(samplerate);
    lpf3.setSampleRate(samplerate);
    lpf4.setSampleRate(samplerate);
    lpf5.setSampleRate(samplerate);
    lpf6.setSampleRate(samplerate);

    lpf1.setCutOffFrequency(1001);
    lpf2.setCutOffFrequency(1002);
    lpf3.setCutOffFrequency(1003);
    lpf4.setCutOffFrequency(104);
    lpf5.setCutOffFrequency(4005);
    lpf6.setCutOffFrequency(7006);
}

float MetalZone::processSample(float input, float gain) {
    float sample = input;

    sample = sample - lpf1.processSample(sample);
    sample = lpf2.processSample(sample);
    sample = lpf3.processSample(sample);

    sample = sample * gain * 1.0f;

    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }

    sample = 0.2f * sample + 0.8f * lpf4.processSample(sample);
    sample = 4.0f * (sample - lpf5.processSample(sample) * 0.7f);
    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }
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