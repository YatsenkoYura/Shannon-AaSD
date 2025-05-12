#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Dictionary {
public:
    void buildFromFile(const std::string& inputPath);

    void writeToFile(const std::string& dictPath) const;

    void readFromFile(const std::string& dictPath);

    std::unordered_map<uint8_t, std::string> encodeMap;

    std::unordered_map<std::string, uint8_t> decodeMap;
};