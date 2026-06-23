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

vector<string> FrequencyCounter::redundancyList(const string& fileName) {
    ifstream file;
    file.exceptions(ifstream::badbit);
    vector<string> frequencyList;

    file.open(fileName);
    if (!file.is_open()) {
        throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
    }

    char curr_char, in;
    int index = 0, count;
    bool exit = false;
    while (!exit) {
        count = 0;
        if (index == 0) file.get(curr_char);
        
        bool same = true;
        while(same) {
            count++;
            if (!file.get(in)) {
                same = false;
                exit = true;
                continue;
            }

            if (in != curr_char) {
                stringstream str;
                str << count;
                if (isdigit(curr_char)) {
                    str << '@';
                }
                str << curr_char;
                frequencyList.push_back(str.str());
                index++;
                curr_char = in;
                same = false;
                continue;
            }
        }
    }

    return frequencyList; 
}
