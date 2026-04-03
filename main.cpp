#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept> 

using namespace std;

/*
    when contributing to this project: the preferred executable name is 'compact',
    as listed in the .gitignore file. This was established as to not flood the repo
    with tons of outdated executable versions of previous c++ code
*/

int main(int argc, char* argv[]) {
    if (argc != 2) {
        // ANSI code to make the text red
        cerr << "\033[31m\nINCORRECT ARGUMENTS\033[0m\n";
        //argv is the arguments vector, argv[0] is the ./executable ( generated in the compilation of the c++ code )
        cerr << "CORRECT USE: " << argv[0] << " <TARGET_FILE_PATH>\n";
        return 1;
    }

    string fileName = argv[1];
    ifstream file;

    //will launch an exception if the file cannot be opened (failbit)
    //or if it detects a reading exception (badbit)
    file.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        file.open(fileName);
        
        cout << "FILE OPENED SUCCESSFULLY: " << fileName << "\n";

        //TODO: implement logic

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