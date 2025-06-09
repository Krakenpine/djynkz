#ifndef _NOTE_CONTAINER_H
#define _NOTE_CONTAINER_H

#include "Note.h"
#include "Sound.h"
#include <vector>

class NoteContainer {
    private:
        std::vector<Note> notes;
        std::string name;

    public:
        NoteContainer();

        Note getNote(int midiNote);
        void addSoundToNote(int midiNote, Sound sound);
        void playNote(int midiNote);
        void stopNote(int midiNote);
        void setName(std::string name_);

        int getAudio();
        std::string getName();
};

#endif // _NOTE_CONTAINER_H