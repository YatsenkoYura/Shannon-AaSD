#include <iostream>
#include <string>
#include "encode/encode.hpp"
#include "decode/decode.hpp"

int main(int argc, char* argv[]) {
    std::string mode = argv[1];
    try {
        if (mode == "encode") {
            encodeFile(argv[2], argv[3], argv[4]);
        }
        else if (mode == "decode") {
            decodeFile(argv[2], argv[3], argv[4]);
        }
        else {
            return 1;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}