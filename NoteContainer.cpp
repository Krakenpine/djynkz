#include "NoteContainer.h"
#include "Note.h"
#include "Sound.h"
#include <vector>

NoteContainer::NoteContainer() : notes(128) {
    for (int i = 0; i < 128; i++) {
        notes[i] = Note(std::to_string(i));
    }
}
Note NoteContainer::getNote(int midiNote) {
    return notes[midiNote];
}

void NoteContainer::addSoundToNote(int midiNote, Sound sound) {
    notes[midiNote].addSound(sound);
}

void NoteContainer::playNote(int midiNote) {
    notes[midiNote].playSound(midiNote);
}

void NoteContainer::stopNote(int midiNote) {
    notes[midiNote].stopWithFadeOut();
}

int NoteContainer::getAudio() {
    int audio = 0;
    for (int i = 0; i < 128; i++) {
        if (notes[i].getIsPlaying()) {
            int tempAudio = notes[i].getNextSample();
            audio += tempAudio;
        }
    }
    return audio;
}


void NoteContainer::setName(std::string name_) {
    name = name_;
}

std::string NoteContainer::getName() {
    return name;
}
