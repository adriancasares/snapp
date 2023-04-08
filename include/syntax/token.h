#ifndef SYNTAX_TOKEN_H_
#define SYNTAX_TOKEN_H_

#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <variant>
#include <vector>

#include "symbol.h"
#include "keyword.h"
#include "sourceLocation.h"

namespace Snapp {

    struct Identifier {
        std::string name;
    };

    std::ostream& operator<<(std::ostream& out, const Identifier& identifier);

    using TokenValue = std::variant<Symbol, Keyword, Identifier, int, double, bool, std::string>;

    class Token {
    public:
        Token(TokenValue value, SourceLocation start, SourceLocation end);

        const TokenValue& value() const;
        const SourceLocation& start() const;
        const SourceLocation& end() const;

    private:
        TokenValue value_;
        SourceLocation start_, end_;
    };

    std::ostream& operator<<(std::ostream& out, const Token& token);

}

#endif /* SYNTAX_TOKEN_H_ */