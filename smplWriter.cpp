#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace std;

void smplWrite(string sampleDirectory, string filename, unsigned char midinote, uint64_t looppoint, unsigned char looptype) {
        cout << sampleDirectory << filename << " " << midinote << " " << looppoint << endl;

        uintmax_t fileSizeSystem = filesystem::file_size(sampleDirectory + filename);

        if (fileSizeSystem == 0) {
            cerr << "File is empty or does not exist." << endl;
            return;
        }
        vector<uint8_t> buf(fileSizeSystem);

        ifstream file(sampleDirectory + filename, ios::binary);
        file.read(reinterpret_cast<char*>(buf.data()), fileSizeSystem);
        if(!file) {
            cerr << "Error reading file." << file.gcount() << " bytes read." << endl;
        }
        file.close();

        std::ofstream newWavData(sampleDirectory + filename.erase(filename.size()-4) + "_smpl.wav", std::ios::binary);

        uint8_t loopstart1 = static_cast<uint8_t>((looppoint) & 0xFF);
        uint8_t loopstart2 = static_cast<uint8_t>((looppoint >> 8) & 0xFF);
        uint8_t loopstart3 = static_cast<uint8_t>((looppoint >> 16) & 0xFF);
        uint8_t loopstart4 = static_cast<uint8_t>((looppoint >> 24) & 0xFF); 
    

        buf.insert(buf.end(), {0x73, 0x6D, 0x70, 0x6C}); // smpl
        buf.insert(buf.end(), {0x3C, 0x00, 0x00, 0x00}); // size
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // manufacturer
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // product
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // sample period
        buf.insert(buf.end(), {midinote, 0x00, 0x00, 0x00}); // midi unity note
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // midi pitch fraction
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // smpte format
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // smpte offset
        buf.insert(buf.end(), {0x01, 0x00, 0x00, 0x00}); // number of sample loops
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // sample data
  
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // loop id
        buf.insert(buf.end(), {looptype, 0x00, 0x00, 0x00}); // loop type
        buf.insert(buf.end(), {loopstart1, loopstart2, loopstart3, loopstart4}); // loop start
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // loop end
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // loop fraction
        buf.insert(buf.end(), {0x00, 0x00, 0x00, 0x00}); // loop play times

        newWavData.write(reinterpret_cast<const char*>(buf.data()), buf.size());
        newWavData.flush();
        newWavData.close();
}