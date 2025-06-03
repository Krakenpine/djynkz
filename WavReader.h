#ifndef _WAV_READER_H
#define _WAV_READER_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

class WavReader {

private:
    std::string filename;
    int sampleRate;
    int numChannels;
    int bitsPerSample;
    int dataSize;
    int loopPoint;
    bool hasLoopFlag;
    bool isStaccatoFlag;
    int midiNote;
    int midiPitchFraction;
    std::vector<int16_t> buffer;


public:
    WavReader(std::string filename, bool skipSmpl);
    ~WavReader();

    bool open();
    void close();
    bool readHeader();
    bool readData(char* buffer, size_t bufferSize, size_t& bytesRead);

    int getSampleRate() const;
    int getNumChannels() const;
    int getBitsPerSample() const;
    int getDataSize() const;
    int getLoopPoint();
    bool hasLoop() const;
    bool isStaccato() const;

    int getMidiNote() const;
    int getMidiPitchFraction() const;

    size_t getBufferSize() const;

    int getSample(size_t index) const;
    float getSampleFloat(size_t index);
};

#endif // _WAV_READER_H