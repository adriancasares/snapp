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
        if (auto* value = std::get_if<Identifier>(&value_)) {
            return *value;
        } else {
            throw SyntaxError("expected an identifier", start_, end_);
        }
    }

    std::ostream& operator<<(std::ostream& out, const Token& token) {
        out << "(" << token.start() << ", ";

        if (auto* value = std::get_if<Symbol>(&token.value())) {
            out << *value;
        }
        else if (auto* value = std::get_if<Keyword>(&token.value())) {
            out << *value;
        }
        else if (auto* value = std::get_if<Identifier>(&token.value())) {
            out << *value;
        }
        else if (auto* value = std::get_if<IntValue>(&token.value())) {
            out << "int{" << *value << "}";
        }
        else if (auto* value = std::get_if<FloatValue>(&token.value())) {
            out << "float{" << *value << "}";
        }
        else if (auto* value = std::get_if<BoolValue>(&token.value())) {
            out << (*value ? "bool{true}" : "bool{false}");
        }
        else if (auto* value = std::get_if<StrValue>(&token.value())) {
            out << "String{\"" << *value << "\"}";
        }
        else {
            out << "<unknown>";
        }

        return out << ")";
    }

}