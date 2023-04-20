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
#include "dataType.h"

namespace Snapp {

    struct Identifier {
        std::string name;
    };

    std::ostream& operator<<(std::ostream& out, const Identifier& identifier);

    class Token {
    public:
        using Value = std::variant<Symbol, Keyword, Identifier, IntValue, FloatValue, BoolValue, StrValue>;

        Token(Value value, SourceLocation start, SourceLocation end);

        const Value& value() const;
        const SourceLocation& start() const;
        const SourceLocation& end() const;

        bool has(const Symbol& symbol) const;
        bool has(const Keyword& keyword) const;
        void expect(const Symbol& symbol) const;
        void expect(const Keyword& keyword) const;
        const Identifier& expectIdentifier() const;

    private:
        Value value_;
        SourceLocation start_, end_;
    };

    std::ostream& operator<<(std::ostream& out, const Token& token);

    using TokenIterator = std::vector<Token>::const_iterator;

}

#endif /* SYNTAX_TOKEN_H_ */