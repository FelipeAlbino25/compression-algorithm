#include "testbench_runner.hpp"
#include <iostream>
#include <stdexcept>

#include "../logic/frequency_counter.hpp" 
#include "character_counter.hpp"


using namespace std;

void TestbenchRunner::run(const string& option, const string& fileName) {
    if (option == "a") {
        cout << "\nFREQUENCY OF CHARACTERS TESTBENCH\n";
        FrequencyCounter uMap;
        CharacterCounter counter;
        
        unordered_map<char, int> freqMap = uMap.countFrequencies(fileName);
        counter.printFrequencies(freqMap);
    } 
}