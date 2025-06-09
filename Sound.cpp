#include "Sound.h"
#include <vector>
#include <math.h>
using namespace std;

Sound::Sound() {
    sampleRate = 0;
    loopPoint = 0;
    hasLoopFlag = false;
    isStaccato = false;
    midiNote = 0;
    midiPitchFraction = 0;
    length = 0;
    readPosition = 0.0f;
    isPlaying = false;
}

void Sound::addData(uint16_t* data_, int size, int samplerate_, int hardwareSamplerate, int midiNote_, bool loopable, int loopPoint_) {
    if (data_ == nullptr || size <= 0) {
        cerr << "Invalid data or size." << endl;
        return;
    }
    data = data_;
    length = size;
    midiNote = midiNote_;
    sampleRate = samplerate_;
    float samplerateDifference = static_cast<float>(hardwareSamplerate) / static_cast<float>(samplerate_);
    float midiFrequency = 440.0f * pow(2.0f, (midiNote_ - 69) / 12.0f);
    initFrequency = midiFrequency * pow(2.0f, midiPitchFraction / 12.0f) * samplerateDifference;
    playFrequency = initFrequency;
    delta = 1;

    hasLoopFlag = loopable;
    isStaccato = !loopable;
    loopPoint = loopPoint_;

    // cout << midiNote << " " << initFrequency << " " << samplerate_ << " " << hardwareSamplerate << endl;

}

void Sound::setReadPosition(float readPosition_) {
    if (readPosition_ < 0 || readPosition_ >= length) {
        readPosition_ = 0;
    }
    readPosition = readPosition_;
}

void Sound::setPlayFrequency(float playFrequency_) {
    playFrequency = playFrequency_;
    delta = playFrequency_ / initFrequency;
}

int Sound::getNextSample() {
    if (!isPlaying) {
        return 0;
    }

    readPosition += delta;

    if (readPosition >= length ) {
        if (isStaccato) {
            readPosition = 0;
            isPlaying = false;
            return 0;
        } else {
            readPosition = loopPoint + readPosition - floor(readPosition);
        }
    }

    double whole = floor(readPosition);
    double decimal = readPosition - whole;

    float lower_sample = static_cast<int16_t>(data[static_cast<int>(whole)]);            
    float upper_sample;
    if (int(whole) + 1 >= length) {
        upper_sample = static_cast<int16_t>(data[loopPoint]);
    } else {
        upper_sample = static_cast<int16_t>(data[int(whole + 1)]);
    }
    float temp = (1.0 - decimal) * lower_sample + decimal * upper_sample;
    int tempInt = static_cast<int>(temp);
    //cout << whole << " " << lowerInt << " " << upperInt << " " << tempInt <<endl;
    return tempInt;
}

void Sound::setSemiNoteShift(int seminote) {
    playFrequency = initFrequency * pow(2.0f, seminote / 12.0f);
    delta = playFrequency / initFrequency;
    cout << name << " setting semitone shift: " << seminote << ", new frequency: " << playFrequency << ", new delta: " << delta << endl;
}

void Sound::setLoopPoint(int loopPoint_) {
    cout << "Setting loopPoint: " << loopPoint_ << endl;
    loopPoint = loopPoint_;
}

void Sound::startNote() {
    readPosition = 0;
    isPlaying = true;
}

void Sound::startNote(int seminote) {
    setSemiNoteShift(seminote);
    startNote();
}

void Sound::startNoteMidiNote(int midiNote_) {
    startNote(midiNote_ - midiNote);
}

void Sound::stopNote() {
    isPlaying = false;
}

uint16_t* Sound::getData() {
    return data;
}

int Sound::getLength() {
    return length;
}

int Sound::getSampleRate() {
    return sampleRate;
}

int Sound::getMidiNote() {
    return midiNote;
}

bool Sound::getIsStaccato() {
    return isStaccato;
}

int Sound::getLoopPoint() {
    return loopPoint;
}

void Sound::setName(std::string name_) {
    name = name_;
}

std::string Sound::getName() const {
    return name;
}

std::string Sound::getFilename() const {
    return filename;
}

void Sound::setFilename(std::string filename_) {
    filename = filename_;
}