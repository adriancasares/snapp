#include "syntax/tokenizer.h"
#include "error/syntaxError.h"

namespace Snapp {

    Tokenizer::Tokenizer(const std::string& source) {
        chars_ = source.begin();
        end_ = source.end();
    }
    
    std::vector<Token> Tokenizer::tokens() const {
        return tokens_;
    }
  
    const SourceLocation& Tokenizer::location() const {
        return location_;
    }
  
    char Tokenizer::nextChar() {
        if (chars_ == end_) {
          return 0;
        }
        char next = *chars_;
        ++chars_;
        if (next == '\n') {
          ++location_.index;
          ++location_.line;
          location_.column = 1;
        } else {
          ++location_.index;
          ++location_.column;
        }
        return next;
    }
  
    char Tokenizer::peekChar() {
        if (chars_ == end_) {
          return 0;
        }
        return *chars_;
    }
  
    void Tokenizer::pushToken(Token token) {
        tokens_.push_back(token);
    }

    std::vector<Token> Tokenizer::tokenize(const std::string& source) {
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
                size_t idx;
                if (isInteger) {
                    try {
                        int number = std::stoi(rawNumber, &idx);
                        if (idx != rawNumber.length()) {
                            throw std::invalid_argument("");
                        }
                        tokenizer.pushToken(Token(number, start, currentEnd));
                    } catch (std::invalid_argument) {
                        throw SyntaxError("invalid integer literal", start, currentEnd);
                    }
                } else {
                    try {
                        double number = std::stod(rawNumber, &idx);
                        if (idx != rawNumber.length()) {
                            throw std::invalid_argument("");
                        }
                        tokenizer.pushToken(Token(number, start, currentEnd));
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