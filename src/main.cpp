#include <fstream>
#include <iostream>

#include "token.h"

int main() {
    std::string sourcePath;
    std::cout << "Source file: ";
    std::cin >> sourcePath;
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
        std::vector<Snapp::Token> tokens = Snapp::getAllTokens(sourceCode);
        std::cout << "Done! :)" << std::endl;
    } catch (Snapp::SyntaxError error) {
        std::cerr << error.output(sourceCode) << std::endl;
    }
}
