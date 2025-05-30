#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>

#include "WavReader.h"
#include "Sound.h"
#include "WavWriter.h"
#include "sequence.cpp"
#include "Note.h"
#include "NoteContainer.h"
#include "smplWriter.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    if (!(argc == 2 || argc == 6)) {
        cout << "Use djynkz.exe 'path' for normal function or djynkz.exe 'path' 'filename' 'midinote' 'looppoint' 'looptype' to add smpl chunk to file" << endl;
        return 0;
    }

    const string sampleDirectory = argv[1];

    cout << sampleDirectory << endl;

    if (argc == 6) {
        string filename = argv[2];
        unsigned char midinote = atoi(argv[3]);
        uint64_t looppoint = atoll(argv[4]);
        unsigned char looptype = atoi(argv[5]);

        smplWrite(sampleDirectory, filename, midinote, looppoint, looptype);
        return 1;
    } 

    static uint16_t sampleBuffer[32000000];

    vector<string> files;

    for (const auto & entry : std::filesystem::directory_iterator(sampleDirectory))
        files.push_back(entry.path().string());


    vector<Sound> allSounds;
    
    size_t ramIndex = 0;
    for (size_t i = 0; i < files.size(); i++) {
        WavReader wavReader(files[i]);
        cout << "Buffer size: " << wavReader.getBufferSize() << endl;

        for (size_t i = 0; i < wavReader.getBufferSize(); i++) {
            sampleBuffer[ramIndex + i] = wavReader.getSample(i); 
        }

        Sound sound;
        cout << "staccato: " << wavReader.isStaccato() << endl;
        sound.addData(&sampleBuffer[ramIndex], wavReader.getBufferSize(), wavReader.getSampleRate(), 48000, wavReader.getMidiNote(), !wavReader.isStaccato());
        sound.setLoopPoint(wavReader.getLoopPoint());

        allSounds.push_back(sound);

        ramIndex += wavReader.getBufferSize();
    }

    NoteContainer sampler;
    NoteContainer samplerStac;

    for (size_t i = 0; i < 128; i++) {
        size_t indexOfClosestSoundNormal = 0; 
        int closestDistanceNormal = 128;
        size_t indexOfClosestSoundStac = 0; 
        int closestDistanceStac = 128;
        for (size_t j = 0; j < allSounds.size(); j++) {
            int distance = abs(allSounds[j].getMidiNote() - (int)i);
            if (allSounds[j].getIsStaccato()) {
                if (distance < closestDistanceStac) {
                    closestDistanceStac = distance;
                    indexOfClosestSoundStac = j;
                }
            } else {
                if (distance < closestDistanceNormal) {
                    closestDistanceNormal = distance;
                    indexOfClosestSoundNormal = j;
                }
            }

        }
        cout << "adding norm sample: " << allSounds[indexOfClosestSoundNormal].getMidiNote() << " to: " << i << endl;
        Sound cloneSoundNormal;
        cloneSoundNormal.addData(allSounds[indexOfClosestSoundNormal].getData(), allSounds[indexOfClosestSoundNormal].getLength(), allSounds[indexOfClosestSoundNormal].getSampleRate(), 48000, allSounds[indexOfClosestSoundNormal].getMidiNote(), !allSounds[indexOfClosestSoundNormal].getIsStaccato());
        sampler.addSoundToNote(i, cloneSoundNormal);
        Sound cloneSoundStac;
        cout << "adding stac sample: " << allSounds[indexOfClosestSoundStac].getMidiNote() << " to: " << i << endl;
        cloneSoundStac.addData(allSounds[indexOfClosestSoundStac].getData(), allSounds[indexOfClosestSoundStac].getLength(), allSounds[indexOfClosestSoundStac].getSampleRate(), 48000, allSounds[indexOfClosestSoundStac].getMidiNote(), !allSounds[indexOfClosestSoundStac].getIsStaccato());
        samplerStac.addSoundToNote(i, cloneSoundStac); 
    }

    // crude sequencer
    vector<float> outputSamples;

    double bpm = tempo;
    double samplerate = 48000.0;
    double sixteenth_note_duration_seconds = 60.0 / (bpm * 4.0);
    int samples_per_sixteenth = static_cast<int>(sixteenth_note_duration_seconds * samplerate);

    int sequenceIndex = 0;
    int sampleCounter = 0;
    int tick = 0;
    
    while (tick <= seqLength) {
        if (sampleCounter % samples_per_sixteenth == 0) {
            if (sequence[sequenceIndex].tick == tick) {
                int howManySequencesThisTick = 0;
                for (int i = 0; i < seqLength; i++) {
                    if (sequence[sequenceIndex + i].tick == tick) {
                        howManySequencesThisTick++;
                    } else { 
                        break;
                    }
                }
                for (int i = 0; i < howManySequencesThisTick; i ++) {
                    if (sequence[sequenceIndex].playNote) {
                        // cout << "play note" << endl; 
                        if (sequence[sequenceIndex].stac) {
                            samplerStac.playNote(sequence[sequenceIndex].note);
                        } else {
                            sampler.playNote(sequence[sequenceIndex].note);
                        }
                    } else {
                        if (sequence[sequenceIndex].stac) {
                            samplerStac.stopNote(sequence[sequenceIndex].note);
                        } else {
                            sampler.stopNote(sequence[sequenceIndex].note);
                        }
                        // cout << "stop note" << endl;
                    }
                    sequenceIndex++;
                } 
            } 
            tick++;
        } 
        sampleCounter++; 
        float sample = float(sampler.getAudio()) / 32767.0f + float(samplerStac.getAudio()) / 32767.0f;

        sample *= 1.0f;

        // fast tanh for soft clipping and limiting signal, crude but sounds better than over- or underflowing integer
        if (sample < -3.f) { sample = -1.f; }
        else if (sample > 3.f) { sample = 1.f; }
        else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }

        outputSamples.push_back(sample * 0.80);
    }

    vector<int16_t> outputSamplesInt; 

    for (size_t i = 0; i < outputSamples.size(); i++) {
        outputSamplesInt.push_back(static_cast<int16_t>(outputSamples[i] * 32767));
    }

    WavWriter writer("newWave.wav", 1, 48000, 16, outputSamplesInt, outputSamplesInt.size());
    writer.WriteFile();

    return 1;
}