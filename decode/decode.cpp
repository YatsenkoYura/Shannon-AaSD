#include "decode.hpp"
#include "../src/dictionary.h"
#include <fstream>
#include <stdexcept>
#include <cstdint>

void decodeFile(const std::string& encodedPath, const std::string& dictPath, const std::string& outputPath) {
    Dictionary dict;
    dict.readFromFile(dictPath);
    std::ifstream in(encodedPath, std::ios::binary);
    std::ofstream out(outputPath, std::ios::binary);

    std::string buffer;
    char ch;
    while (in.get(ch)) {
        uint8_t byte = static_cast<uint8_t>(ch);
        for (int bit = 7; bit >= 0; --bit) {
            buffer.push_back( ((byte >> bit) & 1) ? '1' : '0' );
            auto it = dict.decodeMap.find(buffer);
            if (it != dict.decodeMap.end()) {
                out.put(static_cast<char>(it->second));
                buffer.clear();
            }
        }
    }
}