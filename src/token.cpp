#include "token.h"
#include "tokenizer.cpp"
#include <cctype>
#include <sstream>

namespace Snapp {

    Symbol findSymbol(const std::string& content) {
        auto it = literalToSymbol.find(content);
        return it == literalToSymbol.end() ? Symbol::Unknown : it->second;
    }
    
    std::ostream& operator<<(std::ostream& out, const Symbol& symbol) {
        out << "Symbol::";
        auto it = symbolNames.find(symbol);
        return it == symbolNames.end() ? out << "Unknown" : out << it->second;
    }

    Keyword findKeyword(const std::string& content) {
        auto it = literalToKeyword.find(content);
        return it == literalToKeyword.end() ? Keyword::Unknown : it->second;
    }
    
    std::ostream& operator<<(std::ostream& out, const Keyword& keyword) {
        out << "Keyword::";
        auto it = keywordNames.find(keyword);
        return it == keywordNames.end() ? out << "Unknown" : out << it->second;
    }

    std::ostream& operator<<(std::ostream& out, const SourceLocation& location) {
        return out << "(line " << location.line << ":" << location.column << ")";
    }

    Token::Token(TokenValue value, SourceLocation start, SourceLocation end) {
        value_ = value;
        start_ = start;
        end_ = end;
    }

    const TokenValue& Token::value() const {
        return value_;
    }

    const SourceLocation& Token::start() const {
        return start_;
    }

    const SourceLocation& Token::end() const {
        return end_;
    }

    SyntaxError::SyntaxError() {}

    SyntaxError::SyntaxError(std::string message) {
        message_ = message;
    }

    SyntaxError::SyntaxError(std::string message, std::optional<SourceLocation> start, std::optional<SourceLocation> end) {
        message_ = message;
        start_ = start;
        end_ = end;
    }

    const std::string& SyntaxError::message() const {
        return message_;
    }

    const std::optional<SourceLocation>& SyntaxError::start() const {
        return start_;
    }

    const std::optional<SourceLocation>& SyntaxError::end() const {
        return end_;
    }

    std::string Token::output() const {
      if (std::holds_alternative<Symbol>(value_)) {
        return symbolNames.at(std::get<Symbol>(value_));
      }
      if (std::holds_alternative<Keyword>(value_)) {
        return keywordNames.at(std::get<Keyword>(value_));
      }
      if (std::holds_alternative<Identifier>(value_)) {
          return std::get<Identifier>(value_).name;
      }
      if (std::holds_alternative<Integer>(value_)) {
        return std::to_string(std::get<Integer>(value_));
      }
      if (std::holds_alternative<Float>(value_)) {
        return std::to_string(std::get<Float>(value_));
      }
      if (std::holds_alternative<Bool>(value_)) {
        return std::get<Bool>(value_) ? "true" : "false";
      }
      if (std::holds_alternative<String>(value_)) {
        return std::get<String>(value_);
      }
    }

    std::string SyntaxError::output(const std::string& sourceCode) const {
        std::ostringstream out;
        out << "SyntaxError";
        if (start_) {
            out << " " << *start_;
        }
        if (!message_.empty()) {
            out << ": " << message_;
        }
        // TODO: highlight relevant code segment
        return out.str();
    }

    std::vector<Token> getAllTokens(const std::string& source) {
        Tokenizer tokenizer(source);

        while (char next = tokenizer.peekChar()) {
            SourceLocation start = tokenizer.location();
            if (std::isspace(next)) {
                tokenizer.nextChar();
            } else if (std::isalpha(next) || next == '_') {
                std::string word;
                while (char next = tokenizer.peekChar()) {
                    if (!std::isalnum(next) && next != '_') {
                        break;
                    }
                    word.push_back(tokenizer.nextChar());
                }
                Keyword keyword = findKeyword(word);
                if (keyword != Keyword::Unknown) {
                    tokenizer.pushToken(Token(keyword, start, tokenizer.location()));
                } else if (word == "true") {
                    tokenizer.pushToken(Token(true, start, tokenizer.location()));
                } else if (word == "false") {
                    tokenizer.pushToken(Token(false, start, tokenizer.location()));
                } else {
                    tokenizer.pushToken(Token(Identifier{word}, start, tokenizer.location()));
                }
            } else if (std::isdigit(next)) {
                std::string rawNumber;
                bool isInteger = true;
                std::optional<Token> deferredToken;
                SourceLocation currentEnd = tokenizer.location();
                while (char next = tokenizer.peekChar()) {
                    if (next == '_' || next == '\'') {
                        tokenizer.nextChar();
                        continue;
                    } else if (next == '.') {
                        isInteger = false;
                    } else if (!std::isdigit(next)) {
                        break;
                    }
                    currentEnd = tokenizer.location();
                    rawNumber.push_back(tokenizer.nextChar());
                    if (next == '.') {
                        if (tokenizer.peekChar() == '.') {
                            tokenizer.nextChar();
                            if (tokenizer.peekChar() == '=') {
                                tokenizer.nextChar();
                                deferredToken.emplace(Symbol::RangeInc, currentEnd, tokenizer.location());
                            } else {
                                deferredToken.emplace(Symbol::RangeExc, currentEnd, tokenizer.location());
                            }
                            isInteger = true;
                            rawNumber.pop_back();
                            break;
                        }
                    }
                    currentEnd = tokenizer.location();
                }
                if (isInteger) {
                    try {
                        tokenizer.pushToken(Token(std::stoi(rawNumber), start, currentEnd));
                    } catch (std::invalid_argument) {
                        throw SyntaxError("invalid integer literal", start, currentEnd);
                    }
                } else {
                    try {
                        tokenizer.pushToken(Token(std::stod(rawNumber), start, currentEnd));
                    } catch (std::invalid_argument) {
                        throw SyntaxError("invalid floating-point literal", start, currentEnd);
                    }
                }
                if (deferredToken) {
                    tokenizer.pushToken(*deferredToken);
                }
            } else if (std::ispunct(next)) {
                std::string rawSymbol;
                rawSymbol.push_back(tokenizer.nextChar());
                if (next == '/') {
                    if (tokenizer.peekChar() == '/') {
                        bool ignoreNextNewline = false;
                        while (char next = tokenizer.nextChar()) {
                            if (ignoreNextNewline) {
                                if (next == '\n' || !std::isspace(next)) {
                                    ignoreNextNewline = false;
                                }
                            } else if (next == '\n') {
                                break;
                            } else if (next == '\\') {
                                ignoreNextNewline = true;
                            }
                        }
                        continue;
                    } else if (tokenizer.peekChar() == '*') {
                        bool canClose = false;
                        while (char next = tokenizer.nextChar()) {
                            if (canClose) {
                                if (next == '/') {
                                    break;
                                } else if (next != '*') {
                                    canClose = false;
                                }
                            } else if (next == '*') {
                                canClose = true;
                            }
                        }
                        continue;
                    }
                } else if (next == '"') {
                    std::string literal;
                    bool escaped = false;
                    bool terminated = false;
                    while (char next = tokenizer.nextChar()) {
                        if (escaped) {
                            char escChar = next;
                            switch (next) {
                                case 'n':
                                    escChar = '\n';
                                    break;
                                case 'r':
                                    escChar = '\r';
                                    break;
                                case 't':
                                    escChar = '\t';
                                    break;
                                default:
                                    break;
                            }
                            literal.push_back(escChar);
                        } else if (next == '"') {
                            terminated = true;
                            break;
                        } else if (next == '\\') {
                            escaped = true;
                        } else {
                            literal.push_back(next);
                        }
                    }
                    if (!terminated) {
                        throw SyntaxError("string has no closing quote", start, {});
                    }
                    tokenizer.pushToken(Token(literal, start, tokenizer.location()));
                    continue;
                }
                std::string symbolPeek = rawSymbol;
                while (char next = tokenizer.peekChar()) {
                    symbolPeek.push_back(next);
                    if (findSymbol(symbolPeek) == Symbol::Unknown) {
                        break;
                    }
                    rawSymbol.push_back(tokenizer.nextChar());
                }
                Symbol symbol = findSymbol(rawSymbol);
                if (symbol == Symbol::Unknown) {
                    throw SyntaxError("invalid symbol: " + rawSymbol, start, tokenizer.location());
                }
                tokenizer.pushToken(Token(symbol, start, tokenizer.location()));
            } else {
                tokenizer.nextChar();
                throw SyntaxError(std::string("unexpected character: ") + next, start, tokenizer.location());
            }
        }
        return tokenizer.tokens();
    }

}