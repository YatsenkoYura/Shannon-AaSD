#include "decode.hpp"
#include "../src/dictionary.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cstdint>

void decodeFile(const std::string& encodedPath, const std::string& dictPath, const std::string& outputPath) {

    Dictionary dict;
    dict.readFromFile(dictPath);

    std::ifstream in(encodedPath, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open encoded: " + encodedPath);

    std::ofstream out(outputPath, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open output: "  + outputPath);

    char hdr;
    if (!in.get(hdr))
        return;

    unsigned padBits = static_cast<unsigned char>(hdr);
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());

    size_t N = data.size();
    std::string cur;
    for (size_t i = 0; i < N; ++i) {
        uint8_t byte = data[i];
        int bitsToDo = (i+1 < N ? 8 : padBits);
        for (int b = 7; b >= 8 - bitsToDo; --b) {
            cur.push_back( ((byte >> b)&1) ? '1' : '0' );
            auto it = dict.decodeMap.find(cur);
            if (it != dict.decodeMap.end()) {
                out.put(static_cast<char>(it->second));
                cur.clear();
            }
        }
    }
}