#include "CabinetModeler.h"
#include "WavReader.h"

#include <vector>
#include <math.h>

CabinetModeler::CabinetModeler(float samplerate_) {
    samplerate = samplerate_;
    speed = 1;
}

void CabinetModeler::setType(Cabinet type_) {
    type = type_;

    IRsample.clear();
    IRbuffer.clear();

    std::string filename = "";

    switch (type) {
        case PURE:
            filename = "pure";
            break;
        case FOURxTWELVE_SM57:
            filename = "1960_4x12_57_Close.wav";
            break;
        case MOTOWN:
            filename = "Motown_1x15_409_Mid.wav";
            break;
        case MORPH:
            break;
        default:
            break;
    }

    if (filename == "pure") {
        IRsample.push_back(1);
        IRbuffer.push_back(0);
        for (size_t i = 0; i < 40; i++) {
            IRsample.push_back(0);
            IRbuffer.push_back(0);
        }
    } else if (type != MORPH) {
        WavReader IR(filename, true);
        for (size_t i = 0; i < IR.getBufferSize(); i++) {
            IRbuffer.push_back(0);
            IRsample.push_back(IR.getSampleFloat(i));
        }
    }
}

float CabinetModeler::processSample(float input) {
    float temple = 0;

    IRbuffer[IRbufferIndex] = input;

    float IRsum = 0;

    for (size_t j = 0; j < IRbuffer.size(); j++) {
        int tempIndex = (IRbufferIndex - j + IRbuffer.size()) % IRbuffer.size();

        if (speed == 1) {
            temple += IRbuffer[tempIndex] * IRsample[j];
            IRsum += fabs(IRsample[j]);
        } else {
            float IRsampleIndex = j * speed;
            int indexWhole = floor(IRsampleIndex);
            float indexDecimal = IRsampleIndex - indexWhole;

            if (indexWhole >= IRbuffer.size() - 1) {
                temple += 0;
            } else {
                float fadeOut = 1;
                if (indexWhole > IRbuffer.size() - 100) {
                    fadeOut = 1 - ((IRbuffer.size() - indexWhole) / 100);
                }
                IRsum += fabs(IRsample[indexWhole] + ((1.0 - indexDecimal) * IRsample[indexWhole + 1]) * fadeOut);
                temple += IRbuffer[tempIndex] * ( indexDecimal * IRsample[indexWhole] + ((1.0 - indexDecimal) * IRsample[indexWhole + 1]) * fadeOut);
            }
        }
    }
    
    IRbufferIndex++;
    if (IRbufferIndex >= IRsample.size()) {
        IRbufferIndex = 0;
    }

    float kerroin2 = 1 / IRsum;

    float realoutput2 = temple * kerroin2;

    if (largestSample < fabs(realoutput2)) {
        largestSample = fabs(realoutput2);
    }

    return realoutput2;
}

float CabinetModeler::getLargestSample() {
    return largestSample;
}

std::vector<float> CabinetModeler::getIRsampleArray() {
    return IRsample;
}

void CabinetModeler::setIR(std::vector<float> newIR) {
    IRsample.clear();
    for (int i = 0; i < newIR.size(); i++) {
        IRsample.push_back(newIR[i]);
    }
}

void CabinetModeler::resetBuffer(int length) {
    IRbuffer.clear();
    for (size_t i = 0; i < length; i++) {
        IRbuffer.push_back(0);
    }
}

void CabinetModeler::setSpeed(float speed_) {
    speed = speed_;
}