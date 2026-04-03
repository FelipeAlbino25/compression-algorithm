#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept> 
#include <unordered_map>
#include "testbenches/character_counter.hpp"

using namespace std;

/*
    when contributing to this project: the preferred executable name is 'compact',
    as listed in the .gitignore file. This was established as to not flood the repo
    with tons of outdated executable versions of previous c++ code

    there is a testbench.txt file that will be used for testing each of the algorithm steps
*/

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // ANSI code to make the text red
        cerr << "\033[31m\nINCORRECT ARGUMENTS\033[0m\n";
        //argv is the arguments vector, argv[0] is the ./executable ( generated in the compilation of the c++ code )
        cerr << "CORRECT USE: " << argv[0] << " <TARGET_FILE_PATH>\n (OPTIONAL)<TESTBENCH_OPTIONS>";
        return 1;
    }

    //fileName is argument passed at the terminal
    string fileName = argv[1];
    ifstream file;
    
    //testBenchOptions will receive the argument of the user to activate certain benchmarks
    string testBenchOptions = "";
    if(argc == 3){
        testBenchOptions = argv[2];
    }
    else if(argc >3){
        cerr << "\033[31m\nINCORRECT ARGUMENTS (TOO MANY ARGUMENTS PASSED)\033[0m\n";
        cerr << "CORRECT USE: " << argv[0] << " <TARGET_FILE_PATH>\n (OPTIONAL)<TESTBENCH_OPTIONS>";

    }

    //will launch an exception if it detects a reading error (badbit)
    file.exceptions(fstream::badbit);

    try {
        //new object that offers the count of each character and its visualization
        CharacterCounter counter;
        
        file.open(fileName);

        //detecting failbit manually as to not launch an error everytime the file.get() gets to the end of a text file
        if (!file.is_open()) {
            throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
        }
        
        cout << "FILE OPENED SUCCESSFULLY: " << fileName << "\n";

        //logical structure for counting number of occurences of each character (number/letter/space/etc)
        unordered_map<char,int> frequencyMap = counter.countFrequencies(fileName);

        if(testBenchOptions=="a"){
            counter.printFrequencies(frequencyMap);
        }
    } 
    // verifies if its an ifstream exception
    catch (const ifstream::failure& e) {
        cerr << "\033[31m\nERROR: EXCEPTION OCURRED DURING FILE HANDLING\033[0m\n";
        cerr << "DETAILS: WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: '" << fileName << "'.\n";
        return 1;
    } 
    //capture any other generic exception launched by the program logic
    catch (const exception& e) {
        cerr << "\033[31m\nUNEXPECTED ERROR:\033[0m " << e.what() << "\n";
        return 1;
    }

    return 0;
}