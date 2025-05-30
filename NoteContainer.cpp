#include "NoteContainer.h"
#include "Note.h"
#include "Sound.h"
#include <vector>

NoteContainer::NoteContainer() {
    for (int i = 0; i < 128; i++) {
        Note note(std::to_string(i));
        notes.push_back(note);
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
