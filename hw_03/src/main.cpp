#include "huffman.h"

#include <cstring>
#include <string>
#include <iostream>

enum operation_t {
    CONSTRUCT,
    EXTRACT,
    UNKNOWN
};

int main(int argc, char *argv[]) {
    operation_t operation = operation_t::UNKNOWN;
    std::string in_file, out_file;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-c") == 0)
            operation = operation_t::CONSTRUCT;
        else if (std::strcmp(argv[i], "-u") == 0)
            operation = operation_t::EXTRACT;
        else if (std::strcmp(argv[i], "-f") == 0)
            in_file = argv[++i];
        else
            out_file = argv[++i];
    }

    if (operation == operation_t::CONSTRUCT) {
        HuffmanArchiveCreator creator;
        creator.load_file(in_file);
        creator.write_file(out_file);
    } else {
        HuffmanArchiveExtractor extractor;
        extractor.load_file(in_file);
        extractor.write_file(out_file);
    }
    
    return 0;
}
