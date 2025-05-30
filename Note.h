#ifndef _NOTE_H
#define _NOTE_H

#include <vector>
#include "Sound.h"

class Note {
    private:
        std::string name;
        std::size_t currentSound;
        bool isPlaying;
        float muteCounter;
        std::vector<Sound> sounds;
    
    public:
        Note();
        Note(std::string name_);
        Sound getSound(int index);
        void addSound(Sound sound);
        void playSound(int midiNote);
        void stop();
        void stopWithFadeOut();

        int getNextSample();
        bool getIsPlaying();

        bool hasSounds();
};

#endif // _NOTE_H