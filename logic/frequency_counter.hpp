#ifndef FREQUENCY_COUNTER_HPP
#define FREQUENCY_COUNTER_HPP

#include <string>
#include <unordered_map>
using namespace std;

class FrequencyCounter {
public:
    //default constructor
    FrequencyCounter() = default;

    //method responsible for opening the file and counting each instance of a character
    unordered_map<char, int> countFrequencies(const string& fileName);

};

#endif // FREQUENCY_COUNTER_HPP