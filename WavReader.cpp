#include "WavReader.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
using namespace std;

WavReader::WavReader(string filename_) {
    filename = filename_;
    
    // cout << "Opening file: " << filename << endl;

    uintmax_t fileSizeSystem = filesystem::file_size(filename);

    if (fileSizeSystem == 0) {
        cerr << "File is empty or does not exist." << endl;
        return;
    }
    vector<uint8_t> buf(fileSizeSystem);

    ifstream file(filename, ios::binary);
    file.read(reinterpret_cast<char*>(buf.data()), fileSizeSystem);
    if(!file) {
        cerr << "Error reading file." << file.gcount() << " bytes read." << endl;
    }
    file.close();

    // TO DO: check that these are correct before trying to read data
    // uint8_t RIFF[4] = {buf[0], buf[1], buf[2], buf[3]};
    // uint32_t FileSize = buf[7] << 24 | buf[6] << 16 | buf[5] << 8 | buf[4];
    // uint8_t WAVE[4] = {buf[8], buf[9], buf[10], buf[11]};

    // uint8_t fmt[4] = {buf[12], buf[13], buf[14], buf[15]};
    // uint32_t fmtSize = buf[19] << 24 | buf[18] << 16 | buf[17] << 8 | buf[16];
    // uint16_t type = buf[21] << 8 | buf[20];
    uint16_t channels = buf[23] << 8 | buf[22];
    uint32_t sampleRate_ = buf[27] << 24 | buf[26] << 16 | buf[25] << 8 | buf[24];
    uint16_t bitsPerSample_ = buf[35] << 8 | buf[34];

    int dataChunkStart;

    for (int i = 0; i < 1000; i++) {
        if (buf[i] == 100 && buf[i+1] == 97 && buf[i+2] == 116 && buf[i+3] == 97) { // == "data"
                dataChunkStart = i; // Data chunk starts at byte i
                cout << "dataChunkStart: " << i << endl; 
            }
    }
/*
    if (buf[36] == 100 && buf[37] == 97 && buf[38] == 116 && buf[39] == 97) { // == "data"
        dataChunkStart = 36; // Data chunk starts at byte 44
    } else {
        uint8_t numberOfExtraFormatBytes = buf[36] << 8 | buf[37];
        dataChunkStart = 36 + numberOfExtraFormatBytes; // Data chunk starts after the extra format bytes
    }
*/
    // uint8_t data[4] = {buf[dataChunkStart], buf[dataChunkStart + 1], buf[dataChunkStart + 2], buf[dataChunkStart + 3]};
    uint32_t dataSize_ = buf[dataChunkStart + 7] << 24 | buf[dataChunkStart + 6] << 16 | buf[dataChunkStart + 5] << 8 | buf[dataChunkStart + 4];
  
    int smplOffset = dataChunkStart + 8 + dataSize_;
    uint8_t smplText[4] = {buf[0 + smplOffset], buf[1 + smplOffset], buf[2 + smplOffset], buf[3 + smplOffset]};
    cout << "smplOffset: " << smplOffset << " smplText: " << smplText << " dataSize: " << dataSize_ << endl;
    // uint32_t smplSize = buf[7 + smplOffset] << 24 | buf[6 + smplOffset] << 16 | buf[5 + smplOffset] << 8 | buf[4 + smplOffset];
    // uint32_t manufacturer = buf[11 + smplOffset] << 24 | buf[10 + smplOffset] << 16 | buf[9 + smplOffset] << 8 | buf[8 + smplOffset];
    // uint32_t product = buf[15 + smplOffset] << 24 | buf[14 + smplOffset] << 16 | buf[13 + smplOffset] << 8 | buf[12 + smplOffset];
    // uint32_t samplePeriod = buf[19 + smplOffset] << 24 | buf[18 + smplOffset] << 16 | buf[17 + smplOffset] << 8 | buf[16 + smplOffset];
    uint32_t midiUnityNote = buf[23 + smplOffset] << 24 | buf[22 + smplOffset] << 16 | buf[21 + smplOffset] << 8 | buf[20 + smplOffset];
    uint32_t midiPitchFraction_ = buf[27 + smplOffset] << 24 | buf[26 + smplOffset] << 16 | buf[25 + smplOffset] << 8 | buf[24 + smplOffset];
    // uint32_t smpteFormat = buf[31 + smplOffset] << 24 | buf[30 + smplOffset] << 16 | buf[29 + smplOffset] << 8 | buf[28 + smplOffset];
    // uint32_t smpteOffset = buf[35 + smplOffset] << 24 | buf[34 + smplOffset] << 16 | buf[33 + smplOffset] << 8 | buf[32 + smplOffset];
    uint32_t sampleLoopCount = buf[39 + smplOffset] << 24 | buf[38 + smplOffset] << 16 | buf[37 + smplOffset] << 8 | buf[36 + smplOffset];
    uint32_t samplerDataSize = buf[43 + smplOffset] << 24 | buf[42 + smplOffset] << 16 | buf[41 + smplOffset] << 8 | buf[40 + smplOffset];

    uint32_t start = 0;
    bool loopable = true;

    // cout << "sampleLoopCount: " << sampleLoopCount << endl;
    for (size_t i = 0; i < sampleLoopCount; i++ ) {    // what happens if there are more than one loop? Let's just use the last one for now

        int offset = smplOffset + 44 + i * 24;
        // uint32_t identifier = buf[3 + offset] << 24 | buf[2 + offset] << 16 | buf[1 + offset] << 8 | buf[0 + offset];
        uint32_t type = buf[7 + offset] << 24 | buf[6 + offset] << 16 | buf[5 + offset] << 8 | buf[4 + offset];
        start = buf[11 + offset] << 24 | buf[10 + offset] << 16 | buf[9 + offset] << 8 | buf[8 + offset];
        // uint32_t end = buf[15 + offset] << 24 | buf[14 + offset] << 16 | buf[13 + offset] << 8 | buf[12 + offset];
        // uint32_t fraction = buf[19 + offset] << 24 | buf[18 + offset] << 16 | buf[17 + offset] << 8 | buf[16 + offset];
        // uint32_t playCount = buf[23 + offset] << 24 | buf[22 + offset] << 16 | buf[21 + offset] << 8 | buf[20 + offset];
        
        cout << "type: " << type << endl;
        if (type == 32) {
            loopable = false;
        }
    }

    // cout << "start: " << start << endl;

    // TO DO: have this data to contain information of which sample pack this sample belongs to and is it staccato or sustain type
    if (samplerDataSize > 0) {
        cout << "Sampler Data: ";
        for (uint32_t i = 0; i < samplerDataSize; i++) {
            // cout << to_string((uint8_t) buf[91 + smplOffset + sampleLoopCount * 24 + i]) + " ";
        }
        cout << "\n";
    }

    sampleRate = sampleRate_;
    numChannels = channels;
    bitsPerSample = bitsPerSample_;
    dataSize = dataSize_ / (channels * (bitsPerSample / 8)); // Calculate number of samples based on data size and channels
    loopPoint = start;
    hasLoopFlag = (sampleLoopCount > 0);
    isStaccatoFlag = !loopable;
    midiNote = midiUnityNote;
    midiPitchFraction = midiPitchFraction_;

    int dataStart = dataChunkStart + 8; // Skip the data chunk header

    cout << dataSize << "\n";
    for (int i = 0; i < dataSize; i++) {
        for (int j = 0; j < channels; j++) {    // TO DO: What to do with files that have more than one channel?
            if (bitsPerSample == 8) {
                buffer.push_back(buf[dataStart + i + j * 8] << 8);  // save everything as 16-bit samples
            } else if (bitsPerSample == 16) {
                buffer.push_back(buf[dataStart + i * 2 + 1 + j * 8] << 8 | buf[dataStart + i * 2 + 0 + j * 8]);
            } else if (bitsPerSample == 24) {
                buffer.push_back(buf[dataStart + i * 3 + 2 + j * 8] << 8 | buf[dataStart + i * 1 + 1 + j * 8]); // save everything as 16-bit samples, ignore last byte
            } else if (bitsPerSample == 32) {
                buffer.push_back(buf[dataStart + i * 4 + 3 + j * 8] << 8 | buf[dataStart + i * 4 + 2 + j * 8]); // save everything as 16-bit samples, ignore last two bytes
            }
        }
    }

};

WavReader::~WavReader() {
    buffer.clear();
};

int WavReader::getSampleRate() const {
    return sampleRate;
};

int WavReader::getNumChannels() const {
    return numChannels;
};

int WavReader::getBitsPerSample() const {
    return bitsPerSample;
};

int WavReader::getDataSize() const {
    return dataSize;
};

int WavReader::getLoopPoint() {
    return loopPoint;
};

bool WavReader::hasLoop() const {
    return hasLoopFlag;
};

bool WavReader::isStaccato() const {
    return isStaccatoFlag;
};

int WavReader::getMidiNote() const {
    return midiNote;
};

int WavReader::getMidiPitchFraction() const {
    return midiPitchFraction;
};

size_t WavReader::getBufferSize() const {
    return buffer.size();
};

int WavReader::getSample(size_t index) const {
    if (index < 0 || index >= buffer.size()) {
        cerr << "Index out of bounds: " << index << endl;
        return 0; // or throw an exception
    }
    return buffer[index];
};