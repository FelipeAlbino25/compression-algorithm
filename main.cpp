#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept> 
#include <unordered_map>
#include "testbenches/testbench_runner.hpp" 
#include "logic/compressor.hpp"

using namespace std;

/*
    when contributing to this project: use the Makefile to create a executable out of this code,
    as it prevents tons of old versions to flood the repo
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
        return 1;
    }

    //will launch an exception if it detects a reading error (badbit)
    file.exceptions(fstream::badbit);

    try {
      if(testBenchOptions==""){
        Compressor compressor;
        compressor.compress(fileName);
      }
      else{
        TestbenchRunner runner;
        runner.run(testBenchOptions, fileName);
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