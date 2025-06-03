#include "Distortion.h"
#include "LowPassFilter.h"
#include <cmath>

#define M_PI 3.14159265358979323846

Distortion::Distortion(float samplerate_) {
    samplerate = samplerate_;
    oversample = 1;
    preEq.setSampleRate(samplerate * oversample);
    preEq.setCutOffFrequency(20);
    eq1.setSampleRate(samplerate);
    eq1.setCutOffFrequency(1000);
    eq2.setSampleRate(samplerate);
    eq2.setCutOffFrequency(1000);
    eq3.setSampleRate(samplerate);
    eq3.setCutOffFrequency(1000);

    MetalZone metalzone(samplerate * oversample);
    FIR fir2(samplerate * oversample);
    fir = fir2;
    FIR fir3(samplerate * oversample);
    firpre = fir3;

}

void Distortion::setType(Type type_) {
    type = type_;
    switch (type) {
        case RAW:
            break;
        case CLEAN: 
            break;
        case HARDCLIP:
            break;
        case METALZONE:
            metalzone.setSamplerate(samplerate * oversample);
            break;
        case WAVEFOLDER:
            break;
    }
}

void Distortion::setGain(float gain_) {
    gain = gain_;
}

void Distortion::setEq(int eq, float eqFreq, float eqGain) {
    switch (eq) {
        case 1:
            eq1.setCutOffFrequency(eqFreq);
            eq1Gain = eqGain;
            break;
        case 2:
            eq2.setCutOffFrequency(eqFreq);
            eq2Gain = eqGain;
            break;
        case 3:
            eq3.setCutOffFrequency(eqFreq);
            eq3Gain = eqGain;
            break;
    }
}

float Distortion::processSample(float input) {
    float output = 0;

    
    float sample = input - preEq.processSample(input);

    std::vector<float> outbuf;
        
    switch (type) {
        case RAW:
            outbuf.push_back(processRaw(sample));
            break;
        case CLEAN:
            outbuf.push_back(processClean(sample));
            break;
        case HARDCLIP:
            outbuf.push_back(processHardClip(sample));
            break;
        case METALZONE:
            outbuf.push_back(metalzone.processSample(sample, gain));
            break;
        case WAVEFOLDER:
            outbuf.push_back(processWavefolder(sample));
            break;
        default:
            outbuf.push_back(sample);
    }

    return outbuf[0];
}

float Distortion::processRaw(float input) {
    return input;
}

float Distortion::processClean(float input) {
    float sample = input * gain * 5.0f;
    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }
    return sample;
}

float Distortion::processHardClip(float input) {
    float sample = input * gain;

    if (sample > 1) {
        sample = 1;
    } else if (sample < -1) {
        sample = -1;
    }
    return sample;
}

float Distortion::processWavefolder(float input) {
    float polarity = 1;
    if (input < 0) {
        polarity = -1;
    }

    float sample = fabs(input);

    sample = input * gain;

    for (int i = 0; i < 10; i ++) {
        if (i % 2 == 0) {
            if (sample < 0) {
                sample = -1.0f * sample;
            }
        } else {
            if (sample > 1) {
                sample = 1 - 1.0f * (sample - 1);
            }
        }
    }

    if (sample > 1) {
        sample = 1;
    }

    sample *= polarity;

    return sample;
}

float Distortion::getLargestFirSample() {
    return fir.getLargestSample();
}