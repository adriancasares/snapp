#include <fstream>
#include <iostream>

#include "syntax/abstractSyntaxTree.h"
#include "syntax/tokenizer.h"
#include "error/syntaxError.h"
#include "error/runtimeError.h"
#include "semantics/astRunner.h"

int main(int argc, char** argv) {
    std::string sourcePath;

    if (argc > 1) {
        sourcePath = argv[1];
        std::cout << "Using source file '" << sourcePath << "'" << std::endl;
    } else {
        std::cout << "Enter the path to the source file: ";
        std::cin >> sourcePath;
    }

    bool debugEnabled = false;

    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == "--debug") {
            debugEnabled = true;
            break;
        }
    }

    if (debugEnabled) {
        std::cout << "Debug mode enabled" << std::endl;
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
        std::vector<Snapp::Token> tokens = Snapp::Tokenizer::tokenize(sourceCode);

        auto ast = Snapp::AbstractSyntaxTree::fromTokens(tokens);
        if (debugEnabled) std::cout << *ast << std::endl;

        Snapp::ASTRunner::runAST(*ast, debugEnabled);

    } catch (const Snapp::SyntaxError& error) {
        std::cerr << error.output(sourceCode) << std::endl;
        return 1;
    } catch (const Snapp::RuntimeError& error) {
        std::cerr << error.output() << std::endl;
        return 1;
    }
}
