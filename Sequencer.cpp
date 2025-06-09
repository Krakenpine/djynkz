#include "Sequencer.h"
#include <algorithm>

Sequencer::Sequencer() {}

void Sequencer::setTempo(int tempo_) {
    tempo = tempo_;
}

void Sequencer::setLength(int length_) {
    seqLength = length_;
}

void Sequencer::setSequence(std::vector<SeqEvent> sequence_) {
    sequence = sequence_;
}

void Sequencer::setMainTab(std::string filename) {
    std::vector<std::string> mainData;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        mainData.push_back(line);
    }
    file.close();

    tempo = std::stoi(mainData[0]);

    for (size_t i = 1; i < mainData.size(); i++) {
        std::string filename = mainData[i];
        if (filename.find("guitar") != std::string::npos) {
            setGuitarTab(filename);
        } else if (filename.find("bass") != std::string::npos) {
            setBassTab(filename);
        } else if (filename.find("drums") != std::string::npos) {
            setDrumTab(filename);
        } else {
            throw std::runtime_error("Unknown tab type in file: " + filename);
        }
    }

    std::sort(sequence.begin(), sequence.end(), [](const SeqEvent& a, const SeqEvent& b) {
        if (a.tick != b.tick) {
            return a.tick < b.tick;
        }
        // If tick is the same, put playNote == false before playNote == true
        return !a.playNote && b.playNote;
    });

}

void Sequencer::setGuitarTab(std::string filename) {
    GuitarTab tab = {"", "", "", "", "", "", ""};
    std::vector<std::string> tabData;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        tabData.push_back(line);
    }
    file.close();

    std::string instrumentName = tabData[0];

    for (size_t i = 1; i < tabData.size(); i += 7) {
        tab.str1 += tabData[i + 0];
        tab.str2 += tabData[i + 1];
        tab.str3 += tabData[i + 2];
        tab.str4 += tabData[i + 3];
        tab.str5 += tabData[i + 4];
        tab.str6 += tabData[i + 5];
        tab.pm   += tabData[i + 6];
    }

    size_t tabLength = tab.str1.length();

    int startNotes[6] = {64, 59, 55, 50, 45, 40};
    int prevNote[6] = {0, 0, 0, 0, 0, 0};
    bool preNoteStac[6] = {false, false, false, false, false, false};
    bool stringPlaying[6] = {false, false, false, false, false, false};

    int extraSpaces = 0;

    for (size_t j = 0; j < tabLength; j++) {
        char tabChars[6] = { tab.str1[j], tab.str2[j], tab.str3[j], tab.str4[j], tab.str5[j], tab.str6[j] };
        char pm = tab.pm[j];

        bool stac = pm == '.';

        bool addExtraSpace = false;

        for (size_t i = 0; i < 6; i++) {
            if (tabChars[i] != '-') {
                if (tabChars[i] == 'x') {
                    sequence.push_back({int(j) - extraSpaces, instrumentName, false, prevNote[i], preNoteStac[i]});
                } else if (tabChars[i] == '|') {
                    addExtraSpace = true;
                } else {
                    if (stringPlaying[i]) {
                        sequence.push_back({int(j) - extraSpaces, instrumentName, false, prevNote[i], preNoteStac[i]});
                    }
                    sequence.push_back({int(j) - extraSpaces, instrumentName, true, tabCharToNumber(tabChars[i]) + startNotes[i], stac});
                    stringPlaying[i] = true;
                    prevNote[i] = tabCharToNumber(tabChars[i]) + startNotes[i];
                    preNoteStac[i] = stac;
                }
            }
        }
        if (addExtraSpace) {
            extraSpaces++;
        }
    }
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[0], preNoteStac[0]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[1], preNoteStac[1]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[2], preNoteStac[2]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[3], preNoteStac[3]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[4], preNoteStac[4]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[5], preNoteStac[5]});
    setLength(tabLength);
}

void Sequencer::setBassTab(std::string filename) {
    BassTab tab = {"", "", "", "", "" };
    std::vector<std::string> tabData;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        tabData.push_back(line);
    }
    file.close();

    std::string instrumentName = tabData[0];

    for (size_t i = 1; i < tabData.size(); i += 5) {
        tab.str1 += tabData[i + 0];
        tab.str2 += tabData[i + 1];
        tab.str3 += tabData[i + 2];
        tab.str4 += tabData[i + 3];
        tab.pm   += tabData[i + 4];
    }

    size_t tabLength = tab.str1.length();

    int startNotes[4] = {43, 38, 33, 28 };
    int prevNote[4] = {0, 0, 0, 0 };
    bool preNoteStac[4] = {false, false, false, false };
    bool stringPlaying[4] = {false, false, false, false };

    int extraSpaces = 0;

    for (size_t j = 0; j < tabLength; j++) {
        char tabChars[4] = { tab.str1[j], tab.str2[j], tab.str3[j], tab.str4[j] };
        char pm = tab.pm[j];

        bool stac = pm == '.';

        bool addExtraSpace = false;

        for (size_t i = 0; i < 4; i++) {
            if (tabChars[i] != '-') {
                if (tabChars[i] == 'x') {
                    sequence.push_back({int(j) - extraSpaces, instrumentName, false, prevNote[i], preNoteStac[i]});
                } else if (tabChars[i] == '|') {
                    addExtraSpace = true;
                } else {
                    if (stringPlaying[i]) {
                        sequence.push_back({int(j) - extraSpaces, instrumentName, false, prevNote[i], preNoteStac[i]});
                    }
                    sequence.push_back({int(j) - extraSpaces, instrumentName, true, tabCharToNumber(tabChars[i]) + startNotes[i], stac});
                    stringPlaying[i] = true;
                    prevNote[i] = tabCharToNumber(tabChars[i]) + startNotes[i];
                    preNoteStac[i] = stac;
                }
            }
        }
        if (addExtraSpace) {
            extraSpaces++;
        }
    }
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[0], preNoteStac[0]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[1], preNoteStac[1]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[2], preNoteStac[2]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[3], preNoteStac[3]});
    setLength(tabLength);
}

void Sequencer::setDrumTab(std::string filename) {
    DrumTab tab = {"", "", "", "", "", "", "", ""};
    std::vector<std::string> tabData;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) {
        tabData.push_back(line);
    }
    file.close();

    std::string instrumentName = tabData[0];

    for (size_t i = 1; i < tabData.size(); i += 9) {
        tab.crash += tabData[i + 0];
        tab.hihat += tabData[i + 1];
        tab.ride += tabData[i + 2];
        tab.snare += tabData[i + 3];
        tab.high_tom += tabData[i + 4];
        tab.mid_tom += tabData[i + 5];
        tab.lo_tom   += tabData[i + 6];
        tab.kick   += tabData[i + 7];
    }

    size_t tabLength = tab.crash.length();

    int startNotes[8] = {49, 46, 51, 38, 50, 47, 45, 36};
    int prevNote[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool preNoteStac[8] = {false, false, false, false, false, false, false, false};
    bool stringPlaying[8] = {false, false, false, false, false, false, false, false};

    int extraSpaces = 0;

    for (size_t j = 0; j < tabLength; j++) {
        char tabChars[8] = { tab.crash[j], tab.hihat[j], tab.ride[j], tab.snare[j], tab.high_tom[j], tab.mid_tom[j], tab.lo_tom[j], tab.kick[j]   };

        bool stac = true;

        bool addExtraSpace = false;

        for (size_t i = 0; i < 8; i++) {
            if (tabChars[i] != '-') {
                if (tabChars[i] == 'z') {
                    sequence.push_back({int(j) - extraSpaces, instrumentName, false, prevNote[i], preNoteStac[i]});
                } else if (tabChars[i] == '|') {
                    addExtraSpace = true;
                } else {
                    if (stringPlaying[i]) {
                        sequence.push_back({int(j) - extraSpaces, instrumentName, false, prevNote[i], preNoteStac[i]});
                    }
                    sequence.push_back({int(j) - extraSpaces, instrumentName, true, startNotes[i], stac});
                    stringPlaying[i] = true;
                    prevNote[i] = startNotes[i];
                    preNoteStac[i] = stac;
                }
            }
        }
        if (addExtraSpace) {
            extraSpaces++;
        }
    }
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[0], preNoteStac[0]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[1], preNoteStac[1]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[2], preNoteStac[2]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[3], preNoteStac[3]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[4], preNoteStac[4]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[5], preNoteStac[5]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[6], preNoteStac[6]});
    sequence.push_back({int(tabLength), instrumentName, false, prevNote[7], preNoteStac[7]});
    setLength(tabLength);
}


int Sequencer::tabCharToNumber(char tabChar) {
    if (tabChar >= '0' && tabChar <= '9') {
        return tabChar - '0';
    }
    if (tabChar >= 'A' && tabChar <= 'Z') {
        return 10 + (tabChar - 'A');
    }
    return -1; // Invalid character
}

int Sequencer::getTempo() {
    return tempo;
}

int Sequencer::getLength() {
    return seqLength;
}

SeqEvent Sequencer::getEvent(int index) {
    return sequence[index];
}

std::vector<SeqEvent> Sequencer::getEventsAtTick(int tick) {
    std::vector<SeqEvent> events;

    for (size_t i = 0; i < sequence.size(); i++) {
        if (sequence[i].tick == tick) {
            events.push_back(sequence[i]);
        }
    }

    return events;
}