#include "decompressor.hpp"
#include "frequency_counter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;

// Detect which compression algorithm was used by reading the magic header
static int detectAlgorithm(const unsigned char* data) {
    if (memcmp(data, MAGIC_REDUNDANCY, MAGIC_SIZE) == 0) return 1;
    if (memcmp(data, MAGIC_HUFFMAN_NAIVE, MAGIC_SIZE) == 0) return 2;
    if (memcmp(data, MAGIC_HUFFMAN_OPT, MAGIC_SIZE) == 0) return 3;
    return 0; // Unknown
}

// Decompress a file compressed with redundancy (RLE)
// Format: [count][@]char where count > 1, @ precedes digits
static string decompressRedundancy(const unsigned char* data, int dataLen) {
    stringstream result;
    int i = 0;

    while (i < dataLen) {
        // Check if current position starts with a number (count)
        int count = 0;
        bool hasCount = false;

        while (i < dataLen && data[i] >= '0' && data[i] <= '9') {
            count = count * 10 + (data[i] - '0');
            hasCount = true;
            i++;
        }

        if (!hasCount) {
            count = 1;
        }

        if (i >= dataLen) break;

        char ch;
        if (data[i] == '@') {
            // The '@' prefix means the next byte is a digit character
            i++;
            if (i >= dataLen) break;
            ch = static_cast<char>(data[i]);
        } else {
            ch = static_cast<char>(data[i]);
        }
        i++;

        for (int j = 0; j < count; j++) {
            result.put(ch);
        }
    }

    return result.str();
}

// Decompress a file compressed with Huffman (naive or optimized)
// Format: MAGIC(4) | originalSize(8) | tree(variable) | 0xFF | encoded_bits
static string decompressHuffman(const unsigned char* data, int dataLen) {
    int offset = 0;

    // Read original file size (uint64_t, 8 bytes)
    if (offset + 8 > dataLen) {
        throw runtime_error("CORRUPTED HUFFMAN FILE: missing original size");
    }
    uint64_t originalSize;
    memcpy(&originalSize, &data[offset], sizeof(originalSize));
    offset += sizeof(originalSize);

    // Rebuild the Huffman tree from serialized data
    node* root = HuffmanTree::rebuildTree(data, offset, dataLen);

    if (root == nullptr) {
        throw runtime_error("CORRUPTED HUFFMAN FILE: could not rebuild tree");
    }

    // Skip the 0xFF separator
    if (offset < dataLen && data[offset] == 0xFF) {
        offset++;
    } else {
        throw runtime_error("CORRUPTED HUFFMAN FILE: missing 0xFF separator after tree");
    }

    // Decode the bit stream using the Huffman tree
    stringstream result;
    node* current = root;
    uint64_t decodedCount = 0;

    for (int byteIdx = offset; byteIdx < dataLen && decodedCount < originalSize; byteIdx++) {
        unsigned char byte = data[byteIdx];

        for (int bitIdx = 7; bitIdx >= 0 && decodedCount < originalSize; bitIdx--) {
            int bit = (byte >> bitIdx) & 1;

            if (bit == 0) {
                current = current->esq;
            } else {
                current = current->dir;
            }

            if (current == nullptr) {
                throw runtime_error("CORRUPTED HUFFMAN FILE: invalid bit sequence");
            }

            // Reached a leaf node — output the character
            if (current->esq == nullptr && current->dir == nullptr) {
                result.put(current->c);
                decodedCount++;
                current = root;
            }
        }
    }

    if (decodedCount != originalSize) {
        cerr << "WARNING: decoded " << decodedCount << " bytes but expected " << originalSize << endl;
    }

    return result.str();
}

void Decompressor::decompress(const string& fileName) {
    cout << "\nSTARTING DECOMPRESSION OF: " << fileName << "\n";

    // Read entire compressed file into memory
    ifstream file(fileName, ios::binary | ios::ate);
    if (!file.is_open()) {
        throw runtime_error("WAS NOT ABLE TO READ/OPEN THE GIVEN FILE: " + fileName);
    }

    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        throw runtime_error("ERROR READING COMPRESSED FILE: " + fileName);
    }
    file.close();

    if (fileSize < MAGIC_SIZE) {
        throw runtime_error("FILE TOO SMALL TO BE A VALID COMPRESSED FILE: " + fileName);
    }

    // Detect the algorithm
    int algorithm = detectAlgorithm(buffer.data());

    string decompressed;
    string algoName;

    switch (algorithm) {
        case 1: {
            algoName = "REDUNDANCY (RLE)";
            cout << "Detected compression: " << algoName << "\n";
            decompressed = decompressRedundancy(
                buffer.data() + MAGIC_SIZE,
                fileSize - MAGIC_SIZE
            );
            break;
        }
        case 2: {
            algoName = "NAIVE HUFFMAN";
            cout << "Detected compression: " << algoName << "\n";
            decompressed = decompressHuffman(
                buffer.data() + MAGIC_SIZE,
                fileSize - MAGIC_SIZE
            );
            break;
        }
        case 3: {
            algoName = "OPTIMIZED HUFFMAN";
            cout << "Detected compression: " << algoName << "\n";
            decompressed = decompressHuffman(
                buffer.data() + MAGIC_SIZE,
                fileSize - MAGIC_SIZE
            );
            break;
        }
        default:
            throw runtime_error("UNKNOWN OR CORRUPTED FILE FORMAT: " + fileName + " (no valid magic header found)");
    }

    // Determine the output filename by removing the compression extension
    string outName = fileName;
    // Remove known extensions: .freg, .huff.freg, .hopt.freg
    if (outName.size() > 10 && outName.substr(outName.size() - 10) == ".huff.freg") {
        outName = outName.substr(0, outName.size() - 10);
    } else if (outName.size() > 10 && outName.substr(outName.size() - 10) == ".hopt.freg") {
        outName = outName.substr(0, outName.size() - 10);
    } else if (outName.size() > 5 && outName.substr(outName.size() - 5) == ".freg") {
        outName = outName.substr(0, outName.size() - 5);
    } else {
        outName += ".decompressed";
    }

    // Write the decompressed content
    ofstream outFile(outName, ios::binary);
    if (!outFile.is_open()) {
        throw runtime_error("COULD NOT CREATE OUTPUT FILE: " + outName);
    }

    outFile.write(decompressed.data(), decompressed.size());
    outFile.flush();
    outFile.close();

    cout << "Decompression went successfully!" << endl;
    cout << "Algorithm used: " << algoName << endl;
    cout << "Output file: " << outName << endl;
    cout << "Original size restored: " << decompressed.size() << " bytes\n";
}
