#include "error/syntaxError.h"
#include <sstream>

namespace Snapp {

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

}