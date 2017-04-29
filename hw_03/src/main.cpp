#include "huffman.h"

#include <cstring>
#include <string>
#include <iostream>

enum operation_t {
    CONSTRUCT,
    EXTRACT,
    UNKNOWN
};

class WrongArgumentException {
    std::string argument_;
public:
    WrongArgumentException(std::string argument) {
        argument_ = argument;
    }
    std::string message() {
        return "Unknown argument " + argument_;
    }
};

operation_t operation = operation_t::UNKNOWN;
std::string in_file, out_file;

void parse_args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-c") == 0)
            operation = operation_t::CONSTRUCT;
        else if (std::strcmp(argv[i], "-u") == 0)
            operation = operation_t::EXTRACT;
        else if (std::strcmp(argv[i], "-f") == 0)
            in_file = argv[++i];
        else if (std::strcmp(argv[i], "-o") == 0)
            out_file = argv[++i];
        else
            throw WrongArgumentException(argv[i]);
    }
}

int main(int argc, char *argv[]) {
    try {
        parse_args(argc, argv);
    } catch (WrongArgumentException e) {
        std::cerr << e.message() << std::endl;
        return 2;
    }

    try {
        if (operation == operation_t::CONSTRUCT) {
            HuffmanArchiveCreator creator;
            creator.load_file(in_file);
            creator.write_file(out_file);
        } else {
            HuffmanArchiveExtractor extractor;
            extractor.load_file(in_file);
            extractor.write_file(out_file);
        }
    } catch (InputOutputException e) {
        std::cerr << e.message() << std::endl;
        return 2;
    }
    
    return 0;
}
