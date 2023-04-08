//
// Created by Adrian Casares on 4/7/23.
//

#ifndef SYNTAXERROR_H_
#define SYNTAXERROR_H_

#include <string>
#include <optional>
#include "syntax/sourceLocation.h"

namespace Snapp {
  class SyntaxError {
   public:
    SyntaxError();
    SyntaxError(std::string message);
    SyntaxError(std::string message, std::optional<SourceLocation> start, std::optional<SourceLocation> end);

    const std::string &message() const;
    const std::optional<SourceLocation> &start() const;
    const std::optional<SourceLocation> &end() const;

    std::string output(const std::string &sourceCode) const;

   private:
    std::string message_;
    std::optional<SourceLocation> start_, end_;
  };
}

#endif //SNAPP_INCLUDE_SYNTAXERROR_H_
