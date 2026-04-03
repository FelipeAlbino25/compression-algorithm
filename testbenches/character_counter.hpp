#ifndef CHARACTER_COUNTER_HPP
#define CHARACTER_COUNTER_HPP

#include <string>
#include <unordered_map>
using namespace std;

class CharacterCounter {
public:
    //default constructor
    CharacterCounter() = default;

    //method responsible for opening the file and counting each instance of a character
    unordered_map<char, int> countFrequencies(const string& fileName);

    //method responsible for printing the results of the countFrequencies method
    void printFrequencies(const unordered_map<char, int>& frequencyMap);
};

#endif // CHARACTER_COUNTER_HPP