#ifndef DISTORTION_H
#define DISTORTION_H

#include <vector>
#include "LowPassFilter.h"
#include "MetalZone.h"
#include "FIR.h"

enum Type {
    RAW,
    CLEAN,
    HARDCLIP,
    METALZONE,
    WAVEFOLDER
};

class Distortion {
    private:
        Type type = RAW;
        float gain = 1.0f;
        LowPassFilter preEq;
        LowPassFilter eq1;
        LowPassFilter eq2;
        LowPassFilter eq3;
        float eq1Gain = 1.0f;
        float eq2Gain = 1.0f;
        float eq3Gain = 1.0f;

        float samplerate = 48000;

        MetalZone metalzone;

        FIR firpre;
        FIR fir;

        int oversample = 1;

        float processRaw(float input);
        float processClean(float input);
        float processMetalZone(float input);
        float processHardClip(float input);
        float processWavefolder(float input);

    public:
        Distortion(float samplerate_, int oversample_);
        void setType(Type type_);
        void setGain(float gain_);
        void setEq(int eq, float eqFreq, float eqGain);
        void setSamplerate(float samplerate_);

        float processSample(float input);
        float getLargestFirSample();
};

#endif // DISTORTION_H