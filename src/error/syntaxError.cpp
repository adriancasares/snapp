#include "error/syntaxError.h"

#include <sstream>

namespace Snapp {
  
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
            out << " (" << *start_ << ")";
        }
        if (!message_.empty()) {
            out << ": " << message_;
        }
        if (start_ && start_->index >= 0 && start_->index < sourceCode.length()) {
            out << std::endl << "    ";
            size_t index = start_->index;
            while (index > 0 && sourceCode[index - 1] != '\n') {
                --index;
            }
            std::string highlight;
            while (index < start_->index) {
                highlight.push_back(' ');
                out << sourceCode[index++];
            }
            highlight.push_back('^');
            out << sourceCode[index++];
            while (index < sourceCode.length() && sourceCode[index] != '\n' && sourceCode[index] != '\r') {
                highlight.push_back(end_ && index >= end_->index ? ' ' : '~');
                out << sourceCode[index++];
            }
            out << std::endl << "    " << highlight;
        }
        return out.str();
    }

}