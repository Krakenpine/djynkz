#include "Note.h"

Note::Note() : name(""), currentSound(0), isPlaying(false), soundOrderIndex(0) {};

Note::Note(std::string name_) 
    : name(name_), currentSound(0), isPlaying(false), muteCounter(0), soundOrderIndex(0) {};

Sound Note::getSound(int index) {
    return sounds[index];
}

void Note::addSound(Sound sound) {
    sounds.push_back(sound);
}

void Note::playSound(int midiNote) {

    if (soundOrderIndex >= soundOrder.size() || soundOrder.size() == 0) {
        soundOrderIndex = 0;
        soundOrder.clear();
        std::vector<int> tempOrder;
        for (size_t i = 0; i < sounds.size(); i++) {
            tempOrder.push_back(i);
        }

        while(tempOrder.size() > 0) {
            int randomIndex = rand() % tempOrder.size();
            soundOrder.push_back(tempOrder[randomIndex]);
            tempOrder.erase(tempOrder.begin() + randomIndex);
        }
    }
    currentSound = soundOrder[soundOrderIndex];

    muteCounter = 0;

    sounds[currentSound].startNoteMidiNote(midiNote);
    isPlaying = true;

    soundOrderIndex++;
}

int Note::getNextSample() {
    if (muteCounter <= 0) {
        return sounds[currentSound].getNextSample();
    } else {
        float quietedSample = muteCounter * static_cast<float>(sounds[currentSound].getNextSample());
        muteCounter -= 0.05f;
        if (muteCounter <= 0) {
            sounds[currentSound].stopNote();
            isPlaying = false;
        }
        return static_cast<int>(quietedSample);
    }
}

bool Note::getIsPlaying() {
    return isPlaying;
}

bool Note::hasSounds() {
    return sounds.size() > 0;
}

void Note::stop() {
    sounds[currentSound].stopNote();
    isPlaying = false;
}

void Note::stopWithFadeOut() {
    muteCounter = 1;
}