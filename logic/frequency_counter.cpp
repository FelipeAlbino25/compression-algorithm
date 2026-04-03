#include "frequency_counter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

unordered_map<char, int> FrequencyCounter::countFrequencies(const string& fileName) {
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
