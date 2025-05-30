#include "WavWriter.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

WavWriter::WavWriter(
    std::string filename_,
    int channels_,
    int samplerate_,
    int bitsPerSample_,
    std::vector<int16_t> data_,
    int length_
) {
    filename = filename_;
    channels = channels_;
    samplerate = samplerate_;
    bitsPerSample = bitsPerSample_;
    data = data_;
    length = length_;
}

void WavWriter::WriteFile() {

    int bytesPerSample = 1;
    if (bitsPerSample == 16) {
        bytesPerSample = 2;
    } else if (bitsPerSample == 24) {
        bytesPerSample = 3;
    } else if (bitsPerSample == 32) {
        bytesPerSample = 4;
    }

    uint32_t newDataSize = length * bytesPerSample;

    uint32_t totalFileSize = 36 + 8 + newDataSize; // 36 bytes for the header + 'data' + data size + data

    uint8_t sizeByte1 = static_cast<uint8_t>((totalFileSize) & 0xFF);
    uint8_t sizeByte2 = static_cast<uint8_t>((totalFileSize >> 8) & 0xFF);
    uint8_t sizeByte3 = static_cast<uint8_t>((totalFileSize >> 16) & 0xFF);
    uint8_t sizeByte4 = static_cast<uint8_t>((totalFileSize >> 24) & 0xFF);
    
    std::ofstream newWavData(filename, std::ios::binary);

    std::vector<uint8_t> outBuf;

    // TO DO properly:
    outBuf.insert(outBuf.end(), {0x52, 0x49, 0x46, 0x46}); // 'RIFF'
    outBuf.insert(outBuf.end(), {sizeByte1, sizeByte2, sizeByte3, sizeByte4}); // File size
    outBuf.insert(outBuf.end(), {0x57, 0x41, 0x56, 0x45}); // 'WAVE'
    outBuf.insert(outBuf.end(), {0x66, 0x6D, 0x74, 0x20}); // 'fmt '
    outBuf.insert(outBuf.end(), {0x10, 0x00, 0x00, 0x00}); // fmt chunk size (16 for PCM)
    outBuf.insert(outBuf.end(), {0x01, 0x00}); // Audio format (1 for PCM)
    outBuf.insert(outBuf.end(), {0x01, 0x00}); // Number of channels (1 for mono)
    outBuf.insert(outBuf.end(), {0x80, 0xBB, 0x00, 0x00}); // Sample rate (48000)
    outBuf.insert(outBuf.end(), {0x00, 0x00, 0x00, 0x00}); // Byte rate (placeholder)
    outBuf.insert(outBuf.end(), {0x02, 0x00}); // Block align (2 bytes per sample)
    outBuf.insert(outBuf.end(), {0x10, 0x00, }); // Bits per sample ( 16 bits)
    outBuf.insert(outBuf.end(), {0x64, 0x61, 0x74, 0x61}); // 'data'

    outBuf.insert(outBuf.end(), {static_cast<uint8_t>(newDataSize & 0xFF), 
                                  static_cast<uint8_t>((newDataSize >> 8) & 0xFF), 
                                  static_cast<uint8_t>((newDataSize >> 16) & 0xFF), 
                                  static_cast<uint8_t>((newDataSize >> 24) & 0xFF)}); // Data size

    for (int i = 0; i < length; i++) {
        if (bytesPerSample == 2) {
            outBuf.push_back((data[i]) & 0xFF);
            outBuf.push_back((data[i] >> 8 ) & 0xFF);
        }
    }

    newWavData.write(reinterpret_cast<const char*>(outBuf.data()), outBuf.size());
    newWavData.flush();

    newWavData.close();

}