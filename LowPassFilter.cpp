#include "LowPassFilter.h"
#include <cmath>
#define M_PI 3.14159265358979323846

LowPassFilter::LowPassFilter() {};

LowPassFilter::LowPassFilter(float cutOffFrequency_, float samplerate_) {
    cutOffFrequency = cutOffFrequency_;
    samplerate = samplerate_;
    x = tanf(M_PI * cutOffFrequency / samplerate);
}

float LowPassFilter::processSample(float input) {
    float output = x * input + x * prevInput - (x-1) * prevOutput;
    output /= (x+1);
    prevOutput = output;
    prevInput = input;

    return output;
}

void LowPassFilter::setSampleRate(float samplerate_) {
    samplerate = samplerate_;
    x = tanf(M_PI * cutOffFrequency / samplerate);
}

void LowPassFilter::setCutOffFrequency(float cutOffFrequency_) {
    cutOffFrequency = cutOffFrequency_;
    x = tanf(M_PI * cutOffFrequency / samplerate);
}