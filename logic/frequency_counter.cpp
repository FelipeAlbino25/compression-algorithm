#include "frequency_counter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

unordered_map<char, int> FrequencyCounter::countFrequencies(const string& fileName) {
    ifstream file;
    file.exceptions(ifstream::badbit);
    unordered_map<char, int> frequencyMap;

    file.open(fileName);
    if (!file.is_open()) {
        throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
    }

    char in;
    while(file.get(in)){
        frequencyMap[in]++;
    }

    file.close();
    return frequencyMap; 
}

vector<string> FrequencyCounter::redundancyList(const string& fileName) {
    ifstream file(fileName, std::ios::binary);
    //file.exceptions(ifstream::badbit);
    vector<string> frequencyList;

    if (!file.is_open()) {
        throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
    }

    char curr_char, in;
    int index = 0, count = 0;
    while (file.get(in)) {
        if (index == 0) {
            count++;
            curr_char = in;
            index++;
            continue;
        }

        index++;

        if (in != curr_char) {
            stringstream str;
            if (count > 1) {
                str << count;
            }
            if (curr_char >= '0' && curr_char <= '9') {
                str << '@';
            }
            str << curr_char;
            frequencyList.push_back(str.str());
            curr_char = in;
            count = 1;
            continue;
        } else {
            count++;
        }
    }
    file.close();
    cout << "Exited, index=" << index << endl;

    if (count > 0) {
        stringstream str;
        if (count > 1) {
            str << count;
        }
        if (curr_char >= '0' && curr_char <= '9') {
            str << '@';
        }
        str << curr_char;
        frequencyList.push_back(str.str());
    }

    return frequencyList; 
}

ht::HuffmanTree() {
    this->root = nullptr;
}

vector<node*> priority_list(unordered_map<char, int> freqMap) {
    vector<node*> list;
    
    bool inserted;
    for (const auto& [key, value] : freqMap) {
        int size = list.size();
        inserted = false;
        node* aux = new node;
        aux->c = key;
        aux->freq = value;
        aux->dir = nullptr;
        aux->esq = nullptr;
        aux->control_node = false;
        if (size == 0) {
            list.push_back(aux);
            aux = nullptr;
            continue;
        }

        for (int i = 0; i < size; i++) {
            if (list[i]->freq <= aux->freq) {
                list.insert(list.begin() + i, aux);
                inserted = true;
                break;
            } 
        }

        if (!inserted) {
            list.push_back(aux);
        }

        aux = nullptr;
        continue;
    }

    return list;
}

node* ht::build(vector<node*> plist) {
    node *aux, *curr;

    node* root = plist[0];
    string code = "";

    for (int i = 0; i < plist.size(); i += 2) {
        curr = plist[i];
        curr->dir = plist[i + 2];
        curr->esq = plist[i + 1];

        if (curr->control_node) {
            this->table[plist[i + 1]->c] = code + "1";
        }
        code += "0";
    }

    this->root = root;
    this->showTree();

    return root;
}

node* ht::build_opt(vector<node*> plist) {
    node *aux, *curr;
 // TO DO
    node* root = plist[0];
    string code = "";

    for (int i = 0; i < plist.size(); i += 2) {
        curr = plist[i];
        curr->dir = plist[i + 2];
        curr->esq = plist[i + 1];

        if (curr->control_node) {
            this->table[plist[i + 1]->c] = code + "1";
        }
        code += "0";
    }

    this->root = root;
    this->showTree();

    return root;
}

char ht::getChar(string code) {
    return 'a';
}

string node_to_text(node* n) {
    stringstream nstr;
    nstr << "{";
    nstr << n->control_node;
    nstr << n->c;
    nstr << n->freq;
    nstr << "}";
    return nstr.str();
}

void stringify(node* n, stringstream *ss) {
    if (!n->control_node) {
        *(ss) << node_to_text(n);
        return;
    }
    if (n->esq != nullptr) stringify(n->esq, ss);
    if (n->dir != nullptr) stringify(n->dir, ss);
}

string ht::toString() {
    stringstream str;

    stringify(this->root, &str);

    return str.str();
}

void print_list(vector<node*> plist) {
    for (node* n : plist) {
        printf("[%c | %d]->", n->c, n->freq);
    }
    cout << endl;
}

void ht::showTable() {
    for (const auto& [key, value] : this->table) {
        printf("[char: %c | code: %s]\n", key, value.c_str());
    }
}

void ht::showTree() {
    node* aux = this->root;
    string code = "";
    while (aux->control_node == true) {
        printf("CONTROL_NODE (%s): {freq: %d, esq (1): [char: %c, freq: %d], dir (0): [char: %c, freq: %d, control: %d]}\n",
            code.c_str(), aux->freq, aux->esq->c, aux->esq->freq, aux->dir->c, aux->dir->freq, aux->dir->control_node);
        aux = aux->dir;
        code += "0";
    }
}

void writeEncodedText(const string& texto_original, unordered_map<char, string> table, ofstream& arquivo_saida) {
    unsigned char byte_acumulador = 0;
    int contador_bits = 0;
    // Inserir a tabela no arquivo
    for (char letra : texto_original) {
        string codigo_binario = table[letra];

        for (char bit_char : codigo_binario) {
            
            byte_acumulador = byte_acumulador << 1;

            if (bit_char == '1') {
                byte_acumulador = byte_acumulador | 1;
            }

            contador_bits++;

            if (contador_bits == 8) {
                arquivo_saida.put(byte_acumulador); 
                byte_acumulador = 0;               
                contador_bits = 0;                  
            }
        }
    }

    if (contador_bits > 0) {
        byte_acumulador = byte_acumulador << (8 - contador_bits);
        arquivo_saida.put(byte_acumulador);
    }
}

unordered_map<char, string> ht::getTable() {
    return this->table;
}