#ifndef METAL_ZONE_H
#define METAL_ZONE_H

#include "LowPassFilter.h"

class MetalZone {
    private:
        float gain = 1.0f;
        float samplerate = 48000;
        LowPassFilter lpf1;
        LowPassFilter lpf2;
        LowPassFilter lpf3;
        LowPassFilter lpf4;
        LowPassFilter lpf5;
        LowPassFilter lpf6;

    public:
        MetalZone();
        MetalZone(float samplerate_);
        void setSamplerate(float samplerate_);

        float processSample(float input, float gain);
};

#endif // METAL_ZONE_H