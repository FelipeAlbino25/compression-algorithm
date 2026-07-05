#include "frequency_counter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

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

void ht::gerarTabelaCodigos(node* no_atual, string codigo_acumulado) {
    if (no_atual == nullptr) return;

    // Se for uma folha (letra real), salva o código acumulado
    if (no_atual->esq == nullptr && no_atual->dir == nullptr) {
        this->table[no_atual->c] = codigo_acumulado;
        return;
    }

    // Esquerda ganha 0, Direita ganha 1
    gerarTabelaCodigos(no_atual->esq, codigo_acumulado + "0");
    gerarTabelaCodigos(no_atual->dir, codigo_acumulado + "1");
}

node* ht::build(vector<node*> plist) {
    node *aux, *curr;

    node* root = plist[0];
    string code = "";

    for (size_t i = 0; i < plist.size(); i += 2) {
        curr = plist[i];
        if (i + 1 < plist.size()) curr->esq = plist[i + 1];
        if (i + 2 < plist.size()) curr->dir = plist[i + 2];
    }

    this->root = root;
    this->table.clear();
    gerarTabelaCodigos(this->root, code);
    this->showTree();

    return root;
}
#include <queue>
#include <algorithm>

node* ht::build_opt(vector<node*> plist) {
    if (plist.empty()) return nullptr;

    // 1. Garantir que a lista esteja ordenada da MENOR para a MAIOR frequência
    // (Se a sua já veio da maior para a menor, basta dar um reverse nela)
    std::sort(plist.begin(), plist.end(), [](node* a, node* b) {
        return a->freq < b->freq;
    });

    std::queue<node*> fila_folhas; // Fila 1
    std::queue<node*> fila_pais;   // Fila 2

    // Alimenta a primeira fila com as folhas ordenadas
    for (node* n : plist) {
        n->esq = nullptr;
        n->dir = nullptr;
        n->control_node = false;
        fila_folhas.push(n);
    }

    // Função auxiliar interna para pegar o menor nó entre o topo das duas filas
    auto extrair_menor = [&]() {
        if (fila_folhas.empty()) {
            node* res = fila_pais.front();
            fila_pais.pop();
            return res;
        }
        if (fila_pais.empty()) {
            node* res = fila_folhas.front();
            fila_folhas.pop();
            return res;
        }
        // Se ambas têm nós, compara quem é o menor de verdade
        if (fila_folhas.front()->freq < fila_pais.front()->freq) {
            node* res = fila_folhas.front();
            fila_folhas.pop();
            return res;
        } else {
            node* res = fila_pais.front();
            fila_pais.pop();
            return res;
        }
    };

    // 2. Loop de construção da árvore
    // Roda até que reste apenas 1 nó somado nas duas filas juntas
    while (fila_folhas.size() + fila_pais.size() > 1) {
        node* filho_esq = extrair_menor();
        node* filho_dir = extrair_menor();

        // Cria o nó de controle (pai)
        node* pai = new node();
        pai->c = '#';
        pai->freq = filho_esq->freq + filho_dir->freq;
        pai->control_node = true;
        pai->esq = filho_esq;
        pai->dir = filho_dir;

        // Como a soma sempre cresce, o pai vai direto para o fim da Fila 2
        fila_pais.push(pai);
    }

    // O último nó que sobrou é a nossa raiz balanceada
    this->root = extrair_menor();

    // 3. Gera os códigos na tabela
    this->table.clear();
    string code = "";
    gerarTabelaCodigos(this->root, code);

    this->showTree();
    return this->root;
}

char ht::getChar(string code) {
    return 'a';
}

string node_to_text(node* n) {
    stringstream nstr;
    
    nstr.put(n->control_node ? 1 : 0);
    
    
    nstr.put(n->c);
    
    nstr.write(reinterpret_cast<const char*>(&n->freq), sizeof(n->freq));
    
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
    if (aux == nullptr) return;
    string code = "";
    while (aux != nullptr && aux->control_node == true) {
        if (aux->esq != nullptr && aux->dir != nullptr) {
            printf("CONTROL_NODE (%s): {freq: %d, esq (1): [char: %c, freq: %d], dir (0): [char: %c, freq: %d, control: %d]}\n",
                code.c_str(), aux->freq, aux->esq->c, aux->esq->freq, aux->dir->c, aux->dir->freq, aux->dir->control_node);
        }
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

node* ht::getRoot() {
    return this->root;
}

// Serialize the full tree structure recursively in pre-order.
// Format per node:
//   1 byte: type marker (0x00 = leaf, 0x01 = internal)
//   If leaf: 1 byte for char, 4 bytes for freq (little-endian)
//   If internal: recursively write left then right children
void writeTreeSerialized(node* n, ofstream& out) {
    if (n == nullptr) return;

    if (n->esq == nullptr && n->dir == nullptr) {
        // Leaf node
        out.put(0x00);
        out.put(n->c);
        out.write(reinterpret_cast<const char*>(&n->freq), sizeof(n->freq));
    } else {
        // Internal node
        out.put(0x01);
        writeTreeSerialized(n->esq, out);
        writeTreeSerialized(n->dir, out);
    }
}

// Rebuild tree from serialized data (mirrors writeTreeSerialized)
node* ht::rebuildTree(const unsigned char* data, int& offset, int dataLen) {
    if (offset >= dataLen) return nullptr;

    unsigned char type = data[offset++];

    node* n = new node();
    n->esq = nullptr;
    n->dir = nullptr;

    if (type == 0x00) {
        // Leaf
        n->control_node = false;
        n->c = static_cast<char>(data[offset++]);
        memcpy(&n->freq, &data[offset], sizeof(int));
        offset += sizeof(int);
    } else {
        // Internal
        n->control_node = true;
        n->c = '#';
        n->freq = 0;
        n->esq = rebuildTree(data, offset, dataLen);
        n->dir = rebuildTree(data, offset, dataLen);
        if (n->esq) n->freq += n->esq->freq;
        if (n->dir) n->freq += n->dir->freq;
    }

    return n;
}