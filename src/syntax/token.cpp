#include "syntax/token.h"
#include "error/syntaxError.h"

namespace Snapp {

    std::ostream& operator<<(std::ostream& out, const Identifier& identifier) {
        return out << "Identifier{" << identifier.name << "}";
    }

    Token::Token(Value value, SourceLocation start, SourceLocation end) {
        value_ = value;
        start_ = start;
        end_ = end;
    }

    const Token::Value& Token::value() const {
        return value_;
    }

    const SourceLocation& Token::start() const {
        return start_;
    }

    const SourceLocation& Token::end() const {
        return end_;
    }

    bool Token::has(const Symbol& symbol) const {
        return std::holds_alternative<Symbol>(value_) && std::get<Symbol>(value_) == symbol;
    }

    bool Token::has(const Keyword& keyword) const {
        return std::holds_alternative<Keyword>(value_) && std::get<Keyword>(value_) == keyword;
    }

    void Token::expect(const Symbol& symbol) const {
        if (!has(symbol)) {
            throw SyntaxError("expected symbol '" + symbolNames.at(symbol) + "'", start_, end_);
        }
    }

    void Token::expect(const Keyword& keyword) const {
        if (!has(keyword)) {
            throw SyntaxError("expected keyword '" + keywordNames.at(keyword) + "'", start_, end_);
        }
    }

    const Identifier& Token::expectIdentifier() const {
        if (std::holds_alternative<Identifier>(value_)) {
            return std::get<Identifier>(value_);
        } else {
            throw SyntaxError("expected an identifier", start_, end_);
        }
    }

    std::ostream& operator<<(std::ostream& out, const Token& token) {
        out << "(" << token.start() << ", ";
        const Token::Value& value = token.value();

        if (std::holds_alternative<Symbol>(value)) {
            out << std::get<Symbol>(value);
        }
        else if (std::holds_alternative<Keyword>(value)) {
            out << std::get<Keyword>(value);
        }
        else if (std::holds_alternative<Identifier>(value)) {
            out << std::get<Identifier>(value);
        }
        else if (std::holds_alternative<int>(value)) {
            out << "int{" << std::get<int>(value) << "}";
        }
        else if (std::holds_alternative<double>(value)) {
            out << "float{" << std::get<double>(value) << "}";
        }
        else if (std::holds_alternative<bool>(value)) {
            out << (std::get<bool>(value) ? "bool{true}" : "bool{false}");
        }
        else if (std::holds_alternative<std::string>(value)) {
            out << "String{\"" << std::get<std::string>(value) << "\"}";
        }
        else {
            out << "<unknown>";
        }

        return out << ")";
    }

}