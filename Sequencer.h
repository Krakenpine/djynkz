#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

struct SeqEvent {
    int tick;
    std::string instrument;
    bool playNote;
    int note;
    bool stac;
};

struct GuitarTab {
    std::string str1;
    std::string str2;
    std::string str3;
    std::string str4;
    std::string str5;
    std::string str6;
    std::string pm;
};

struct BassTab {
    std::string str1;
    std::string str2;
    std::string str3;
    std::string str4;
    std::string pm;
};

struct DrumTab {
    std::string crash;
    std::string hihat;
    std::string ride;
    std::string snare;
    std::string high_tom;
    std::string mid_tom;
    std::string lo_tom;
    std::string kick;
};

class Sequencer {
    private:
        int tempo = 100;
        int seqLength = 80;
        std::vector<SeqEvent> sequence;

        int tabCharToNumber(char tabChar);

    public:
        Sequencer();

        SeqEvent getNextEvent();
        SeqEvent getEvent(int index);
        std::vector<SeqEvent> getEventsAtTick(int tick);

        void setTempo(int tempo_);
        void setLength(int length_);
        void setSequence(std::vector<SeqEvent> sequence_);
        void setMainTab(std::string filename);
        void setGuitarTab(std::string filename);
        void setBassTab(std::string filename);
        void setDrumTab(std::string filename);

        int getTempo();
        int getLength();
};

#endif // SEQUENCER_H