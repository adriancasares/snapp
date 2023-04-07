#include "tokenizer.h"
#include "token.h"

namespace Snapp {
  Tokenizer::Tokenizer(const std::string& source) {
    chars_ = source.cbegin();
    end_ = source.cend();
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
}