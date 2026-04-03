#include "character_counter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

unordered_map<char, int> CharacterCounter::countFrequencies(const string& fileName) {
    ifstream file;
    file.exceptions(ifstream::badbit);
    unordered_map<char, int> frequencyMap;

    file.open(fileName);
    if (!file.is_open()) {
        throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
    }

    char in;
    while(file.get(in)){
        frequencyMap[in]++;
    }

    return frequencyMap; 
}

void CharacterCounter::printFrequencies(const unordered_map<char, int>& frequencyMap) {
    cout << "\n--- FREQUENCY TABLE ---\n";
    for (auto const& par : frequencyMap) {
        char c = par.first;
        int freq = par.second;

        if (c == '\n') {
            cout << "[\\n] (LINE_BREAK) : " << freq << "\n";
        } else if (c == ' ') {
            cout << "[ ] (SPACE) : " << freq << "\n";
        } else if (c == '\t') {
            cout << "[\\t] (TABULATION) : " << freq << "\n";
        } else {
            cout << "[" << c << "] : " << freq << "\n";
        }
    }
    cout << "-----------------------------\n";
}