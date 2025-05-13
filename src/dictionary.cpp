#include "dictionary.h"
#include <vector>
#include <fstream>
#include <algorithm>

static void shannonFano(
    const std::vector<std::pair<uint8_t, size_t>>& symbols,
    int l, int r,
    const std::string& prefix,
    std::unordered_map<uint8_t, std::string>& enc,
    std::unordered_map<std::string, uint8_t>& dec
) {
    if (l > r) return;
    if (l == r) {
        enc[symbols[l].first] = prefix.empty() ? "0" : prefix;
        dec[enc[symbols[l].first]] = symbols[l].first;
        return;
    }
    size_t total = 0;
    for (int i = l; i <= r; ++i) total += symbols[i].second;
    size_t sum = 0;
    int split = l;
    for (; split < r; ++split) {
        sum += symbols[split].second;
        if (sum >= total - sum) break;
    }
    shannonFano(symbols, l, split, prefix + '0', enc, dec);
    shannonFano(symbols, split + 1, r, prefix + '1', enc, dec);
}

void Dictionary::buildFromFile(const std::string& inputPath) {
    std::ifstream in(inputPath, std::ios::binary);
    std::vector<size_t> freq(256, 0);
    uint8_t byte;
    while (in.read(reinterpret_cast<char*>(&byte), 1)) freq[byte]++;
    std::vector<std::pair<uint8_t, size_t>> symbols;
    for (int i = 0; i < 256; ++i) if (freq[i]) symbols.emplace_back((uint8_t)i, freq[i]);
    std::sort(symbols.begin(), symbols.end(),
              [](auto &a, auto &b) { return a.second > b.second; });
    encodeMap.clear();
    decodeMap.clear();
    if (!symbols.empty()) shannonFano(symbols, 0, int(symbols.size()) - 1, "", encodeMap, decodeMap);
}

void Dictionary::writeToFile(const std::string& dictPath) const {
    std::ofstream out(dictPath);
    out << encodeMap.size() << "\n";
    for (const auto& kv : encodeMap)
        out << int(kv.first) << " " << kv.second << "\n";
}

void Dictionary::readFromFile(const std::string& dictPath) {
    std::ifstream in(dictPath);
    encodeMap.clear();
    decodeMap.clear();
    size_t n;
    in >> n;
    for (size_t i = 0; i < n; ++i) {
        int sym; std::string code;
        in >> sym >> code;
        encodeMap[(uint8_t)sym] = code;
        decodeMap[code] = (uint8_t)sym;
    }
}