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
            // Write magic header for redundancy
            arquivo.write(reinterpret_cast<const char*>(MAGIC_REDUNDANCY), MAGIC_SIZE);
            arquivo.write(final_str.data(), final_str.size());
            cout << "Compression went successfully!" << endl;
            arquivo.flush();
            arquivo.close();
        } else {
            throw runtime_error("ERROR WHILE TRYING TO CREATE COMPRESSED FILE!");
            return;
        }
    } else if (option == "-huffman-naive") {
        cout << "\nCOMPRESSION BY (NAIVE) HUFFMAN\n";
        FrequencyCounter uMap;

        unordered_map<char, int> freqMap = uMap.countFrequencies(fileName);

        vector<node*> plist = priority_list(freqMap);

        int final_size = (2 * plist.size()) - 1;
        int control_nodes = plist.size() - 1;

        int curr_index = 0;
        while(plist.size() < final_size) {
            node* aux = new node;
            aux->c = 'x';
            aux->freq = 0;
            aux->dir = nullptr;
            aux->esq = nullptr;
            aux->control_node = true;
            plist.insert(plist.begin() + curr_index, aux);
            aux = nullptr;
            curr_index += 2;
        }

        for (int i = plist.size() - 1; i >= 0; i -= 2) {
            node* curr = plist[i];
            if (curr->control_node && (size_t)(i + 2) < plist.size()) {
                curr->freq = plist[i + 1]->freq + plist[i + 2]->freq;
                continue;
            }
        }
        
        ht tree = HuffmanTree();
        node* r = tree.build(plist);

        tree.showTable();
        std::ifstream file(fileName, std::ios::binary);
        if (!file.is_open()) {
            throw runtime_error("FAILED TO OPEN FILE #\n");
            return;
        }

        string content;
        char ch;
        while(file.get(ch)) content += ch;
        file.close();

        stringstream cpt_name;
        cpt_name << fileName << ".huff.freg";
        std::ofstream arquivo(cpt_name.str(), std::ios::binary);
        if (arquivo.is_open()) {
            // Write magic header
            arquivo.write(reinterpret_cast<const char*>(MAGIC_HUFFMAN_NAIVE), MAGIC_SIZE);
            // Write original file size (8 bytes, uint64_t)
            uint64_t originalSize = content.size();
            arquivo.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));
            // Write serialized tree structure
            writeTreeSerialized(tree.getRoot(), arquivo);
            // Write separator byte to mark end of tree
            arquivo.put(0xFF);
            // Write encoded data
            writeEncodedText(content, tree.getTable(), arquivo);
            arquivo.flush();
            arquivo.close();
            cout << "Compression went successfully!" << endl;
            return;
        } else {
            throw runtime_error("ERROR WHILE TRYING TO CREATE COMPRESSED FILE!");
            return;
        }
    } else if (option == "-huffman") {
        cout << "\nCOMPRESSION BY HUFFMAN\n";
        FrequencyCounter uMap;

        unordered_map<char, int> freqMap = uMap.countFrequencies(fileName);

        vector<node*> plist = priority_list(freqMap);
        
        ht tree = HuffmanTree();
        node* r = tree.build_opt(plist);

        tree.showTable();
        std::ifstream file(fileName, std::ios::binary);
        if (!file.is_open()) {
            throw runtime_error("FAILED TO OPEN FILE #\n");
            return;
        }

        string content;
        char ch;
        while(file.get(ch)) content += ch;
        file.close();

        stringstream cpt_name;
        cpt_name << fileName << ".hopt.freg";
        std::ofstream arquivo(cpt_name.str(), std::ios::binary);
        if (arquivo.is_open()) {
            // Write magic header
            arquivo.write(reinterpret_cast<const char*>(MAGIC_HUFFMAN_OPT), MAGIC_SIZE);
            // Write original file size (8 bytes, uint64_t)
            uint64_t originalSize = content.size();
            arquivo.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));
            // Write serialized tree structure
            writeTreeSerialized(tree.getRoot(), arquivo);
            // Write separator byte to mark end of tree
            arquivo.put(0xFF);
            // Write encoded data
            writeEncodedText(content, tree.getTable(), arquivo);
            arquivo.flush();
            arquivo.close();
            cout << "Compression went successfully!" << endl;
            return;
        } else {
            throw runtime_error("ERROR WHILE TRYING TO CREATE COMPRESSED FILE!");
            return;
        }
    }
}