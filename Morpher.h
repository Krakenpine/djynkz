#ifndef MORPHER_H
#define MORPHER_H

#include <vector>

struct morph {
    float x;
    float y;
    int id;
};

class Morpher {
    private:
        std::vector<morph> morphee1;
        std::vector<morph> morphee2;
        std::vector<morph> morphed;

        std::vector<float> morphee1IR;
        std::vector<float> morphee2IR;
        std::vector<float> morphedIR;


    public:
        Morpher() {}

        void setMorphee(std::vector<float> inputArray, int selectMorphee);
        void calculateMorphed(float weight);

        float getMorphedSample(int index);
        float getMorphee1Sample(int index);
        float getMorphee2Sample(int index);

        std::vector<float> getMorphee1IR();
        std::vector<float> getMorphee2IR();
        std::vector<float> getMorphedIR();
};


#endif // MORPHER_H