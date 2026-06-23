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
    ifstream file(fileName, std::ios::binary);
    //file.exceptions(ifstream::badbit);
    vector<string> frequencyList;

    if (!file.is_open()) {
        throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
    }

    char curr_char, in;
    int index = 0, count = 0;
    while (file.get(in)) {
        if (index == 0) {
            count++;
            curr_char = in;
            index++;
            continue;
        }

        index++;

        if (in != curr_char) {
            stringstream str;
            if (count > 1) {
                str << count;
            }
            if (curr_char >= '0' && curr_char <= '9') {
                str << '@';
            }
            str << curr_char;
            frequencyList.push_back(str.str());
            curr_char = in;
            count = 1;
            continue;
        } else {
            count++;
        }
    }
    file.close();
    cout << "Exited, index=" << index << endl;

    if (count > 0) {
        stringstream str;
        if (count > 1) {
            str << count;
        }
        if (curr_char >= '0' && curr_char <= '9') {
            str << '@';
        }
        str << curr_char;
        frequencyList.push_back(str.str());
    }

    return frequencyList; 
}
