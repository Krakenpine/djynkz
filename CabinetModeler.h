#ifndef CABINET_MODELER_H
#define CABINET_MODELER_H

#include <filesystem>
#include <vector>


enum Cabinet {
    PURE,
    FOURxTWELVE_SM57
};

class CabinetModeler {
    private:
        Cabinet type = PURE;
        float samplerate;
        std::vector<float> IRsample;
        std::vector<float> IRbuffer;
        size_t IRbufferIndex = 0;
        float largestSample = 0;

    public:
        CabinetModeler(float samplerate_);
        void setType(Cabinet type_);

        float processSample(float input);

        float getLargestSample();
};

#endif // CABINET_MODELER_H