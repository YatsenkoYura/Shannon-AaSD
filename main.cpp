#include <iostream>
#include <string>
#include "encode/encode.hpp"
#include "decode/decode.hpp"

int main(int argc, char* argv[]) {

    if (argc < 5) {
        std::cerr << "Usage: <program> <-e/-d> <input> <output> <key>\n";
        return 1;
    }
    
    std::string mode = argv[1];
    try {
        if (mode == "-e") {
            encodeFile(argv[2], argv[3], argv[4]);
        }
        else if (mode == "-d") {
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