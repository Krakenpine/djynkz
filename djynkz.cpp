#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>

#include "WavReader.h"
#include "Sound.h"
#include "WavWriter.h"
#include "Note.h"
#include "NoteContainer.h"
#include "smplWriter.cpp"
#include "LowPassFilter.h"
#include "Distortion.h"
#include "CabinetModeler.h"
#include "Morpher.h"
#include "Sequencer.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (!(argc == 2 || argc == 6)) {
        std::cout << "Use djynkz.exe 'path' for normal function or djynkz.exe 'path' 'filename' 'midinote' 'looppoint' 'looptype' to add smpl chunk to file" << endl;
    }

    const string sampleDirectory = argv[1];
    //const string sampleDirectory = "E:\\koodaus\\djynkz\\samplet";

    std::cout << sampleDirectory << endl;

    if (argc == 6) {
        string filename = argv[2];
        unsigned char midinote = atoi(argv[3]);
        uint64_t looppoint = atoll(argv[4]);
        unsigned char looptype = atoi(argv[5]);

        smplWrite(sampleDirectory, filename, midinote, looppoint, looptype);
        return 1;
    }

    static uint16_t sampleBuffer[32000000];

    vector<Sound> allSounds;

    vector<string> directories;

    vector<string> soundNames;

    for (const auto& entry : std::filesystem::directory_iterator(sampleDirectory)) {
        if (entry.is_directory()) {
            directories.push_back(entry.path().string());
        }
    }

    size_t ramIndex = 0;
    for (const auto& dir : directories) {
        std::cout << "Directory: " << dir << std::endl;
        vector<string> files;

        string soundName = dir.substr(dir.find_last_of("\\/") + 1);
        soundNames.push_back(soundName);

        for (const auto & entry : std::filesystem::directory_iterator(dir))
            files.push_back(entry.path().string());
        

        for (size_t i = 0; i < files.size(); i++) {
            WavReader wavReader(files[i], false);
            std::cout << "Buffer size: " << wavReader.getBufferSize() << endl;

            for (size_t i = 0; i < wavReader.getBufferSize(); i++) {
                sampleBuffer[ramIndex + i] = wavReader.getSample(i); 
            }

            Sound sound;
            std::cout << "staccato: " << wavReader.isStaccato() << endl;
            
            sound.addData(&sampleBuffer[ramIndex], wavReader.getBufferSize(), wavReader.getSampleRate(), 48000, wavReader.getMidiNote(), !wavReader.isStaccato(), wavReader.getLoopPoint());
            sound.setLoopPoint(wavReader.getLoopPoint());

            if (wavReader.isStaccato()) {
                sound.setName(soundName + "_stac");
            } else {
                sound.setName(soundName);
            }

            sound.setFilename(files[i]);

            allSounds.push_back(sound);

            ramIndex += wavReader.getBufferSize();
        }
    }

    vector<NoteContainer> samplers;

    for (size_t i = 0; i < soundNames.size(); i++) {
        NoteContainer samplerTemp1;
        samplerTemp1.setName(soundNames[i]);
        samplers.push_back(samplerTemp1);
        NoteContainer samplerTemp2;
        samplerTemp2.setName(soundNames[i] + "_stac");
        samplers.push_back(samplerTemp2);
    }

    for (size_t sampler_index = 0; sampler_index < samplers.size(); sampler_index++) {
        
        for (int midinote = 0; midinote < 128; midinote++) {
            vector<int> soundIndices;
            if (samplers[sampler_index].getName().find("drum") != string::npos) { // No neighbouring notes for drums
                for (size_t sound_index = 0; sound_index < allSounds.size(); sound_index++) {
                    if (allSounds[sound_index].getMidiNote() == midinote &&
                        allSounds[sound_index].getName() == samplers[sampler_index].getName()) {
                        soundIndices.push_back(sound_index);
                    }
                }
            } else {
                for (size_t sound_index = 0; sound_index < allSounds.size(); sound_index++) {
                    if ((allSounds[sound_index].getMidiNote() == midinote || 
                        allSounds[sound_index].getMidiNote() == midinote - 1 || 
                        allSounds[sound_index].getMidiNote() == midinote - 2 || 
                        allSounds[sound_index].getMidiNote() == midinote + 1 || 
                        allSounds[sound_index].getMidiNote() == midinote + 2) &&
                        allSounds[sound_index].getName() == samplers[sampler_index].getName()) {
                        soundIndices.push_back(sound_index);
                    }
                }
            }


            if (soundIndices.size() == 0) {
                size_t indexOfClosestSound = 0; 
                int closestDistance = 128;
                for (size_t sound_index = 0; sound_index < allSounds.size(); sound_index++) {
                    if (allSounds[sound_index].getName() == samplers[sampler_index].getName()) {
                        int distance = abs(allSounds[sound_index].getMidiNote() - (int)midinote);
                        if (distance < closestDistance) {
                            closestDistance = distance;
                            indexOfClosestSound = sound_index;
                        }
                    }
                }
                soundIndices.push_back(indexOfClosestSound);
            }
            for (size_t sound_index = 0; sound_index < soundIndices.size(); sound_index++) {
                int index = soundIndices[sound_index];
                std::cout << "adding sample: " << allSounds[index].getFilename() << " " << allSounds[index].getMidiNote() << " to " << samplers[sampler_index].getName() << " to note " << midinote << endl;
                Sound cloneSound;
                cloneSound.addData(allSounds[index].getData(), allSounds[index].getLength(), allSounds[index].getSampleRate(), 48000, allSounds[index].getMidiNote(), !allSounds[index].getIsStaccato(), allSounds[index].getLoopPoint());
                cloneSound.setName(allSounds[index].getName());
                cloneSound.setFilename(allSounds[index].getFilename());
                samplers[sampler_index].addSoundToNote(midinote, cloneSound);
            }
        }


    }

    // crude sequencer
    vector<float> outputSamples;

    Sequencer seq;
    seq.setMainTab("song.txt");

    double bpm = seq.getTempo();
    double samplerate = 48000.0;
    double sixteenth_note_duration_seconds = 60.0 / (bpm * 4.0);
    int samples_per_sixteenth = static_cast<int>(sixteenth_note_duration_seconds * samplerate);

    int sampleCounter = 0;
    int tick = 0;

    // Distortion 
    Distortion distortion1(samplerate, 1); 
    distortion1.setType(METALZONE);
    distortion1.setGain(200);
    // Convolution
    CabinetModeler cabinet1(samplerate); 
    cabinet1.setType(FOURxTWELVE_SM57);
    
    while (tick <= seq.getLength()) {
        if (sampleCounter % samples_per_sixteenth == 0) {
            std::vector<SeqEvent> events = seq.getEventsAtTick(tick);

            for (size_t i = 0; i < events.size(); i++) {
                string eventInstrument = events[i].instrument;

                if (events[i].stac) {
                    eventInstrument += "_stac";
                }

                int samplerIndex = -1;

                for (size_t j = 0; j < samplers.size(); j++) {
                    if (samplers[j].getName() == eventInstrument) {
                        samplerIndex = j;
                        break;
                    }
                }

                if (events[i].playNote) {
                    // cout << "play note" << endl;
                    samplers[samplerIndex].playNote(events[i].note);
                } else {
                    // cout << "stop note" << endl;
                    samplers[samplerIndex].stopNote(events[i].note);
                } 
            }
            tick++;
        } 
        sampleCounter++;
        float sample = 0.f;

        for (size_t i = 0; i < samplers.size(); i++) {
            if (samplers[i].getName() == "guitar1" || samplers[i].getName() == "guitar1_stac") {
                sample += cabinet1.processSample(distortion1.processSample(static_cast<float>(samplers[i].getAudio())/ 32767.0f))  / static_cast<float>(samplers.size()/2);
            }
            if (samplers[i].getName() == "bass" || samplers[i].getName() == "bass_stac") {
                sample += (float(samplers[i].getAudio()) / 32767.0f ) / static_cast<float>(samplers.size()/2);
            }
            if (samplers[i].getName() == "drums" || samplers[i].getName() == "drums_stac") {
                sample += 2.0f * (float(samplers[i].getAudio()) / 32767.0f ) / static_cast<float>(samplers.size()/2);
            }
        }


        // fast tanh for soft clipping and limiting signal, crude but sounds better than over- or underflowing integer
        if (sample < -3.f) { sample = -1.f; }
        else if (sample > 3.f) { sample = 1.f; }
        else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }

        outputSamples.push_back(sample);
    }

    vector<int16_t> outputSamplesInt; 

    for (size_t i = 0; i < outputSamples.size(); i++) {
        outputSamplesInt.push_back(static_cast<int16_t>(outputSamples[i] * 32767));
    }

    WavWriter writer("newWave.wav", 1, 48000, 16, outputSamplesInt, outputSamplesInt.size());
    writer.WriteFile();

    std::cout << "It has been written!" << endl;

    return 1; 
}