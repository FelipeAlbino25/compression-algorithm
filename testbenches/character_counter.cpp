#include "character_counter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;


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