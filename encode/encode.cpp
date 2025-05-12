#include "encode.hpp"
#include "../src/dictionary.h"
#include <fstream>
#include <cstdint>

void encodeFile(const std::string& inputPath, const std::string& outputPath, const std::string& dictPath) {
    Dictionary dict;
    dict.buildFromFile(inputPath);
    dict.writeToFile(dictPath);
    std::ifstream in(inputPath,  std::ios::binary);
    std::ofstream out(outputPath, std::ios::binary);
    uint8_t buffer = 0;
    int bitCount = 0;
    auto flushBuffer = [&](){
        if (bitCount > 0) {
            out.put(static_cast<char>(buffer));
            buffer = 0;
            bitCount = 0;
        }
    };
    uint8_t sym;
    while (in.read(reinterpret_cast<char*>(&sym), 1)) {
        const std::string &code = dict.encodeMap[sym];
        for (char c : code) {
            buffer |= (uint8_t)((c == '1') ? 1 : 0) << (7 - bitCount);
            ++bitCount;
            if (bitCount == 8) {
                out.put(static_cast<char>(buffer));
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    flushBuffer();
}