#include "Morpher.h"

#include <vector>
#include <math.h>

#define M_PI 3.14159265358979323846

float distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Morpher::setMorphee(std::vector<float> inputArray, int selectMorphee) {

    std::vector<morph> tempMorph;

    float lengthOfLine = 0;
    float inputArrayLength = inputArray.size();

    // calculate length of the line of the wave
    for (size_t i = 0; i < inputArray.size() - 1; i++) {
        lengthOfLine += sqrt(1 + pow((inputArray[i] - inputArray[i+1]), 2.0f));
    }

    float stepLength = lengthOfLine / inputArrayLength;

    // set first step
    tempMorph.push_back({0, inputArray[0], 0});

    int arrayTarget = 0;

    // calculate other steps
    for (size_t i = 0; i < inputArray.size() - 1; i++) {
        float startingX = tempMorph[i].x;
        float startingY = tempMorph[i].y;
    
        int j = 1;
        while (distance(startingX, startingY, arrayTarget +j , inputArray[arrayTarget + j]) < stepLength && arrayTarget + j < inputArray.size()) {
            j++;
        }

        // now we know that point in array[i+j] is further, or exactly at, than stepLength from morphee[i] is
        // draw line towards that and take coordinates that are stepLength from morphee[i]

        float angle = atan2((inputArray[i+j] - startingY), j) * (2.0 / M_PI);
        float y_away = angle * stepLength;
        float x_away = stepLength - fabs(y_away);

        tempMorph.push_back({startingX + x_away, startingY + y_away, int(i)});

        arrayTarget += j;
    }

    std::vector<float> tempIR;
    tempIR.push_back(tempMorph[0].y);

    for (size_t i = 1; i < tempMorph.size(); i++) {
        int tempX = floor(tempMorph[i].x);
        if (tempX == floor(tempMorph[i-1].x)) {
            if (fabs(tempMorph[i].y) > fabs(tempIR[i-1])) {
                tempIR[i-1] = tempMorph[i].y;
            }
        } else if (tempX == floor(tempMorph[i-1].x) + 1) {
            tempIR.push_back(tempMorph[i].y);
        } else {
            float steps = floor(tempMorph[i].x) - i;
            for (int i = 0; i < steps; i++) {
                float tempInterpol = ((1.0 + float(i)) / float(steps) * tempMorph[i-1].y)  + ((1.0f - (1.0 + float(i)) / float(steps)) * tempMorph[i].y);
                tempIR.push_back(tempInterpol);
            }
        }
    }

    // TODO: handle IRs with different lengths

    if (selectMorphee == 1) {
        morphee1 = tempMorph;
        morphee1IR = tempIR;
    } else {
        morphee2 = tempMorph;
        morphee2IR = tempIR;
    }

    float inputarraylen = inputArray.size();
    float morphee1len = morphee1.size();
    float morphee1irlen = morphee1IR.size();
}

std::vector<float> Morpher::getMorphee1IR() {
    return morphee1IR;
}

std::vector<float> Morpher::getMorphee2IR() {
    return morphee2IR;
}

std::vector<float> Morpher::getMorphedIR() {
    return morphedIR;
}

void Morpher::calculateMorphed(float weight) {
    morphed.clear();
    morphedIR.clear();
    for (size_t i = 0; i < morphee1.size(); i++) {
        float tempX = morphee1[i].x * (1.0f - weight) + morphee2[i].x * weight;
        float tempY = morphee1[i].y * (1.0f - weight) + morphee2[i].y * weight;

        morphed.push_back({tempX, tempY, int(i)});
    }

    morphedIR.push_back(morphed[0].y);

    for (size_t i = 1; i < morphed.size(); i++) {
        int tempX = floor(morphed[i].x);
        if (tempX == floor(morphed[i-1].x)) {
            if (fabs(morphed[i].y) > fabs(morphedIR[i-1])) {
                morphedIR[i-1] = morphed[i].y;
            }
        } else if (tempX == floor(morphed[i-1].x) + 1) {
            morphedIR.push_back(morphed[i].y);
        } else {
            float steps = floor(morphed[i].x) - i;
            for (int i = 0; i < steps; i++) {
                float tempInterpol = ((1.0 + float(i)) / float(steps) * morphed[i-1].y)  + ((1.0f - (1.0 + float(i)) / float(steps)) * morphed[i].y);
                morphedIR.push_back(tempInterpol);
            }
        }
    }

    while (morphedIR.size() < 1200) {
        morphedIR.push_back(0);
    }
}