#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

class LowPassFilter {
    private:
        float prevOutput = 0;
        float prevInput = 0;
        float samplerate = 48000;
        float cutOffFrequency = 1000;
        float x = 0;

    public:
        LowPassFilter();
        LowPassFilter(float cutOffFrequency_, float samplerate_);
        float processSample(float input);

        void setSampleRate(float samplerate_);
        void setCutOffFrequency(float cutOffFrequency_);
};

#endif // LOW_PASS_FILTER_H