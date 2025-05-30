#ifndef _WAV_WRITER_H
#define _WAV_WRITER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

class WavWriter {
    private:
        int fileSize;
        int channels;
        int samplerate;
        int bitsPerSample;
        std::vector<int16_t> data;
        int length;
        std::string filename;

    public:
        WavWriter(std::string filename_, int channels_, int samplerate_, int bitsPerSample_, std::vector<int16_t> data_, int length_);

        void WriteFile();

};

#endif // _WAV_WRITER_H