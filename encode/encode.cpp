#include "encode.hpp"
#include "../src/dictionary.h"
#include <fstream>
#include <stdexcept>
#include <cstdint>

void encodeFile(const std::string& inputPath, const std::string& outputPath, const std::string& dictPath) {
    Dictionary dict;
    dict.buildFromFile(inputPath);
    dict.writeToFile(dictPath);

    std::ifstream  in(inputPath,  std::ios::binary);
    std::ofstream  out(outputPath, std::ios::binary);
    if (!in)  throw std::runtime_error("Cannot open input: "  + inputPath);
    if (!out) throw std::runtime_error("Cannot open output: " + outputPath);

    // 1 байт заголовок — зарезервируем под padBits
    out.put(char(0));

    uint8_t  buffer  = 0;
    int      bitCnt  = 0;
    int      padBits = 0;

    auto flushBuffer = [&](){
        if (bitCnt > 0) {
            padBits = bitCnt;
            out.put(static_cast<char>(buffer));
        }
        else {
            padBits = 8;
        }
    };

    uint8_t sym;
    while (in.read(reinterpret_cast<char*>(&sym), 1)) {
        const std::string &code = dict.encodeMap[sym];
        for (char c : code) {
            buffer |= (uint8_t)((c=='1')?1:0) << (7 - bitCnt);
            ++bitCnt;
            if (bitCnt == 8) {
                out.put(static_cast<char>(buffer));
                buffer = 0;
                bitCnt = 0;
            }
        }
    }

    flushBuffer();
    out.seekp(0, std::ios::beg);
    out.put(static_cast<char>(padBits));
}