#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include <string>

class Decompressor {
public:
    //decompresses given file, auto-detecting the algorithm used
    void decompress(const std::string& fileName);
};

#endif
