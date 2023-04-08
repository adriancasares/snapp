#include <fstream>
#include <iostream>

#include "syntax/tokenizer.h"
#include "error/syntaxError.h"

int main(int argc, char** argv) {
    std::string sourcePath;

    if (argc > 1) {
        sourcePath = argv[1];
        std::cout << "Using source file '" << sourcePath << "'" << std::endl;
    } else {
        std::cout << "Enter the path to the source file: ";
        std::cin >> sourcePath;
    }

    std::cout << std::endl;
    std::string sourceCode;
    {
        std::ifstream source(sourcePath);
        if (!source.is_open()) {
            std::cerr << "Error: unable to read file '" << sourcePath << "'" << std::endl;
            return 1;
        }
        sourceCode.assign(std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>());
    }

    try {
        std::vector<Snapp::Token> tokens = Snapp::tokenize(sourceCode);
        for (const Snapp::Token& token : tokens) {
            std::cout << token << std::endl;
        }
    } catch (Snapp::SyntaxError error) {
        std::cerr << error.output(sourceCode) << std::endl;
    }
}
