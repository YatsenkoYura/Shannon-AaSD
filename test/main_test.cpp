#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

#include "decode/decode.hpp"
#include "encode/encode.hpp"

TEST(EncodeSmokeTest, CreatesFiles) {
    const std::string in   = "tmp_input.bin";
    const std::string out  = "tmp_output.shn";
    const std::string dict = "tmp_dict.txt";

    {
        std::ofstream fi(in, std::ios::binary);
        ASSERT_TRUE(fi.is_open());
        const std::string data = "Hello, Shannon!";
        fi.write(data.data(), data.size());
    }

    ASSERT_NO_THROW(encodeFile(in, out, dict));

    {
        std::ifstream fo(out, std::ios::binary|std::ios::ate);
        ASSERT_TRUE(fo.is_open());
        EXPECT_GT(fo.tellg(), 0);
    }

    {
        std::ifstream fd(dict);
        ASSERT_TRUE(fd.is_open());
        int lines = 0;
        std::string line;
        while (std::getline(fd, line))
            ++lines;
        EXPECT_GT(lines, 0);
    }

    std::remove(in.c_str());
    std::remove(out.c_str());
    std::remove(dict.c_str());
}

TEST(DecodeSmokeTest, RoundTrip) {
    const std::string in     = "tmp_rt_input.bin";
    const std::string enc    = "tmp_rt_output.shn";
    const std::string dict   = "tmp_rt_dict.txt";
    const std::string out    = "tmp_rt_decoded.bin";

    const std::string payload = "RoundTrip Test Data 12345";
    {
        std::ofstream fi(in, std::ios::binary);
        ASSERT_TRUE(fi.is_open());
        fi.write(payload.data(), payload.size());
    }

    ASSERT_NO_THROW(encodeFile(in, enc, dict));

    ASSERT_NO_THROW(decodeFile(enc, dict, out));

    std::ifstream fd(out, std::ios::binary|std::ios::ate);
    ASSERT_TRUE(fd.is_open());
    std::streamsize sz = fd.tellg();
    ASSERT_GT(sz, 0);
    fd.seekg(0, std::ios::beg);
    std::string decoded(sz, '\0');
    fd.read(decoded.data(), sz);

    EXPECT_EQ(decoded, payload);

    std::remove(in.c_str());
    std::remove(enc.c_str());
    std::remove(dict.c_str());
    std::remove(out.c_str());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}