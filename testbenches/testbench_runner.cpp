#include "testbench_runner.hpp"
#include <iostream>
#include <stdexcept>

#include "../logic/frequency_counter.hpp" 
#include "character_counter.hpp"


using namespace std;
namespace fs = std::filesystem;

void TestbenchRunner::run(const string& option, const string& fileName) {
    if (option == "-count") {
        cout << "\nFREQUENCY OF CHARACTERS TESTBENCH\n";
        FrequencyCounter uMap;
        CharacterCounter counter;
        
        unordered_map<char, int> freqMap = uMap.countFrequencies(fileName);
        counter.printFrequencies(freqMap);
    } else if (option == "-redundancy") {
        cout << "\nCOMPRESSION BY REDUNDANCY\n";
        FrequencyCounter uMap;
        vector<string> list = uMap.redundancyList(fileName);
        stringstream compressed_content;
        for (string s : list) {
            compressed_content << s;
        }
        stringstream cpt_name;
        cpt_name << fileName << ".freg";
        std::ofstream arquivo(cpt_name.str(), std::ios::binary);
        string final_str = compressed_content.str();
        if (arquivo.is_open()) {
            arquivo.write(final_str.data(), final_str.size());
            cout << "Compression went successfully!" << endl;
            arquivo.flush();
            arquivo.close();
        } else {
            throw runtime_error("ERROR WHILE TRYING TO CREATE COMPRESSED FILE!");
            return;
        }

        

    }
}