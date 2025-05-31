#include "CabinetModeler.h"
#include "WavReader.h"

#include <vector>

CabinetModeler::CabinetModeler(float samplerate_) {
    samplerate = samplerate_;
}

void CabinetModeler::setType(Cabinet type_) {
    type = type_;

    std::string filename = "";

    switch (type) {
        case PURE:
            break;
        case FOURxTWELVE_SM57:
            filename = "1960_4x12_57_Close.wav";
            break;
        default:
            break;
    }

    if (filename == "") {
        IRsample.push_back(32000);
        IRbuffer.push_back(0);
    } else {
        WavReader IR(filename, true);
        for (size_t i = 0; i < IR.getBufferSize(); i++) {
            IRbuffer.push_back(0);
            IRsample.push_back(IR.getSample(i));
        }
    }
}

float CabinetModeler::processSample(float input) {
    float temple = 0;

    IRbuffer[IRbufferIndex] = input;

    for (size_t j = 0; j < IRbuffer.size(); j++) {
        int tempIndex = IRbufferIndex - j;
        if (tempIndex < 0) {
            tempIndex = IRbuffer.size() - j;
        }
        temple += float(IRbuffer[tempIndex]) * float(IRsample[j]);
    }

    if (largestSample < abs(temple)) {
        largestSample = abs(temple);
    }
    
    IRbufferIndex++;
    if (IRbufferIndex >= IRsample.size()) {
        IRbufferIndex = 0;
    }

    return temple;
}

float CabinetModeler::getLargestSample() {
    return largestSample;
}
