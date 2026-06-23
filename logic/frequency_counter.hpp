#ifndef FREQUENCY_COUNTER_HPP
#define FREQUENCY_COUNTER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
using namespace std;

class FrequencyCounter {
public:
    //default constructor
    FrequencyCounter() = default;

    //method responsible for opening the file and counting each instance of a character
    unordered_map<char, int> countFrequencies(const string& fileName);
    //method responsible for generating the redundancy list of a file
    vector<string> redundancyList(const string& fileName);
};

struct Node {
    bool control_node;
    char c;
    int freq;
    struct Node* dir;
    struct Node* esq;
} typedef node;

class HuffmanTree {
    private:
        node* root;
        unordered_map<char, string> table;
    
    public:
        HuffmanTree();
        node* build(vector<node*> plist);
        char getChar(string code);
        string stringify();
        void showTable();
        void showTree();
        unordered_map<char, string> getTable();

} typedef ht;

vector<node*> priority_list(unordered_map<char, int> freqMap);

void print_list(vector<node*> plist);

void writeEncodedText(const string& texto_original, unordered_map<char, string> table, ofstream& arquivo_saida);
#endif // FREQUENCY_COUNTER_HPP