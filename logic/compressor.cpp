#include "compressor.hpp"
#include <iostream>

#include "frequency_counter.hpp" 


using namespace std;

void Compressor::compress(const string& fileName) {
    cout << "STARTING COMPRESSION OF: " << fileName << "\n";

    // 1. count the frequency of each character
    FrequencyCounter counter;
    auto freqMap = counter.countFrequencies(fileName);

    //TODO: rest of the compression logic

    cout << "COMPRESSION FINISHED\n";
}