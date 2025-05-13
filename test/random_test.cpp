#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <random>
#include <vector>

#include "encode/encode.hpp"
#include "decode/decode.hpp"

static void writeBin(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot write file: " + path);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
}

static std::vector<uint8_t> readBin(const std::string& path) {
    std::ifstream in(path, std::ios::binary|std::ios::ate);
    if (!in) throw std::runtime_error("Cannot open file: " + path);
    auto sz = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<uint8_t> buf(sz);
    in.read(reinterpret_cast<char*>(buf.data()), sz);
    return buf;
}

TEST(RandomDataTest, RoundTrip) {
    std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> lenDist(0, 4096);
    std::uniform_int_distribution<int> byteDist(0, 255);

    const std::string f_in   = "rnd_input.bin";
    const std::string f_enc  = "rnd_output.shn";
    const std::string f_dict = "rnd_dict.txt";
    const std::string f_out  = "rnd_decoded.bin";

    for (int iter = 0; iter < 20; ++iter) {
        size_t len = lenDist(rng);
        std::vector<uint8_t> data(len);
        for (size_t i = 0; i < len; ++i)
            data[i] = static_cast<uint8_t>(byteDist(rng));

        ASSERT_NO_THROW(writeBin(f_in, data));

        ASSERT_NO_THROW(encodeFile(f_in, f_enc, f_dict));

        ASSERT_NO_THROW(decodeFile(f_enc, f_dict, f_out));

        std::vector<uint8_t> decoded;
        ASSERT_NO_THROW(decoded = readBin(f_out));

        // 5) сравнить
        EXPECT_EQ(decoded, data) << "Mismatch on iteration " << iter;

        std::remove(f_in.c_str());
        std::remove(f_enc.c_str());
        std::remove(f_dict.c_str());
        std::remove(f_out.c_str());
    }
}
