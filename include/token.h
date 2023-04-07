#ifndef TOKEN_H
#define TOKEN_H

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

    using Integer = int;
    using Float = double;
    using Bool = bool;
    using String = std::string;

    struct Identifier {
        std::string name;
    };

    using TokenValue = std::variant<Symbol, Keyword, Identifier, Integer, Float, Bool, String>;

    class Token {
      public:
          Token(TokenValue value, SourceLocation start, SourceLocation end);

          std::string output() const;
          const TokenValue& value() const;
          const SourceLocation& start() const;
          const SourceLocation& end() const;

      private:
          TokenValue value_;
          SourceLocation start_, end_;
    };

    std::vector<Token> getAllTokens(const std::string& source);

}

#endif