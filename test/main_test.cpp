#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdio>
#include <stdexcept>

#include "encode/encode.hpp"
#include "decode/decode.hpp"
#include "src/dictionary.h"

static std::vector<char> readAll(const std::string& path) {
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    auto pos = in.tellg();
    if (pos < 0)
        throw std::runtime_error("tellg failed: " + path);
    in.seekg(0, std::ios::beg);
    std::vector<char> buf((size_t)pos);
    in.read(buf.data(), pos);
    return buf;
}

TEST(EndToEndTest, Sample1) {
    namespace FS = std::filesystem;
    FS::path datadir("test/src");
    std::string in   = (datadir / "sample1.bin").string();
    std::string dict = "tmp_dict.txt";
    std::string enc  = "tmp_out.shn";
    std::string dec  = "tmp_decoded.bin";

    ASSERT_NO_THROW(encodeFile(in, enc, dict));

    Dictionary got, exp;
    ASSERT_NO_THROW(got.readFromFile(dict));
    ASSERT_NO_THROW(exp.readFromFile((datadir / "sample1.txt").string()));
    EXPECT_EQ(got.encodeMap, exp.encodeMap);

    std::vector<char> gotBits, expBits;
    ASSERT_NO_THROW(gotBits = readAll(enc));
    ASSERT_NO_THROW(expBits = readAll((datadir / "sample1.shn").string()));
    EXPECT_EQ(gotBits, expBits);

    ASSERT_NO_THROW(decodeFile(enc, dict, dec));
    std::vector<char> outDec, original;
    ASSERT_NO_THROW(outDec   = readAll(dec));
    ASSERT_NO_THROW(original = readAll(in));
    EXPECT_EQ(outDec, original);

    std::remove(dict.c_str());
    std::remove(enc.c_str());
    std::remove(dec.c_str());
    std::cout << "[ INFO ] Removed temporary files:\n"
              << "  " << dict << "\n"
              << "  " << enc  << "\n"
              << "  " << dec  << "\n";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}