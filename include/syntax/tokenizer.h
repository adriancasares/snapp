#ifndef SYNTAX_TOKENIZER_H_
#define SYNTAX_TOKENIZER_H_

#include <string>
#include <vector>

#include "token.h"

namespace Snapp {

    class Tokenizer {
    public:
        Tokenizer(const std::string& source);

        const SourceLocation& location() const;
        char nextChar();
        char peekChar();

        void pushToken(Token token);
        std::vector<Token> tokens() const;

        static std::vector<Token> tokenize(const std::string& source);

    private:
        std::string::const_iterator chars_, end_;
        SourceLocation location_;
        std::vector<Token> tokens_;
    };

}

#endif /* SYNTAX_TOKENIZER_H_ */
