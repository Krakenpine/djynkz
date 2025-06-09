#ifndef _SOUND_H
#define _SOUND_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

class Sound {
private:
    int sampleRate;
    int loopPoint;
    bool hasLoopFlag;
    bool isStaccato;
    int midiNote;
    int midiPitchFraction;
    int length;
    uint16_t* data;
    float initFrequency;
    float playFrequency;
    double delta;
    double readPosition;
    bool isPlaying;
    std::string name;
    std::string filename;

public:
    Sound();

    int getSample(int index);
    int getMidiNote() const;
    int getNextSample();
    int getSampleRate();
    int getMidiNote();
    int getSize();
    std::string getName() const;
    std::string getFilename() const;

    uint16_t* getData();

    int getLength();

    bool getIsPlaying();

    void addData(uint16_t* data_, int size, int samplerate_, int hardwareSamplerate, int midiNote_, bool loopable, int loopPoint_);
    void setReadPosition(float readPosition_);
    void setPlayFrequency(float playFrequency_);
    void setLoopPoint(int loopPoint_);

    void setSemiNoteShift(int seminote);

    void startNote();
    void startNote(int seminote);
    void startNoteMidiNote(int midiNote_);
    void stopNote();

    bool getIsStaccato();

    int getLoopPoint();

    void setName(std::string name_);
    void setFilename(std::string filename_);
};

#endif // _SOUND_H