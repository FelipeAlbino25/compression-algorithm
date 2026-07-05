#ifndef FREQUENCY_COUNTER_HPP
#define FREQUENCY_COUNTER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
using namespace std;

// Magic bytes to identify compression type in compressed files
// Each compressed file starts with these 4 bytes
const unsigned char MAGIC_REDUNDANCY[]  = {0xC0, 0x01, 0x00, 0x01}; // Redundancy RLE
const unsigned char MAGIC_HUFFMAN_NAIVE[] = {0xC0, 0x01, 0x00, 0x02}; // Naive Huffman
const unsigned char MAGIC_HUFFMAN_OPT[]  = {0xC0, 0x01, 0x00, 0x03}; // Optimized Huffman
const int MAGIC_SIZE = 4;

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

string node_to_text(node* n);

class HuffmanTree {
    private:
        node* root;
        unordered_map<char, string> table;
    
    public:
        HuffmanTree();
        node* build(vector<node*> plist);
        node* build_opt(vector<node*> plist);
        char getChar(string code);
        string toString();
        void showTable();
        void showTree();
        unordered_map<char, string> getTable();
        void gerarTabelaCodigos(node* no_atual, string codigo_acumulado);
        node* getRoot();

        // Rebuild a Huffman tree from a serialized binary stream
        static node* rebuildTree(const unsigned char* data, int& offset, int dataLen);

} typedef ht;

vector<node*> priority_list(unordered_map<char, int> freqMap);

void print_list(vector<node*> plist);

void writeEncodedText(const string& texto_original, unordered_map<char, string> table, ofstream& arquivo_saida);

void stringify(node* n, stringstream* ss);

// Serialize the full tree structure (including internal nodes) so it can be rebuilt for decompression
void writeTreeSerialized(node* n, ofstream& out);
#endif // FREQUENCY_COUNTER_HPP