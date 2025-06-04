#ifndef FIR_H
#define FIR_H

#include <filesystem>
#include <vector>

class FIR {
    private:
        float samplerate = 48000;
        std::vector<float> FIRsample;
        std::vector<float> FIRbuffer;
        size_t FIRbufferIndex = 0;
        float largestSample = 0;
        int oversampling = 1;

    public:
        FIR();
        FIR(float samplerate_);
        void setSamplerate(float samplerate_);

        float processSample(float input);

        float getLargestSample();
        void setOversampling(int oversampling_);
};

#endif // FIR_H