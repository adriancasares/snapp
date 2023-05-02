#include "error/runtimeError.h"

#include <sstream>

namespace Snapp {
  
    RuntimeError::RuntimeError(std::string message) {
        message_ = message;
    }
  
    const std::string& RuntimeError::message() const {
        return message_;
    }
  
    std::string RuntimeError::output() const {
        std::ostringstream out;
        out << "RuntimeError";
        if (!message_.empty()) {
            out << ": " << message_;
        }
        return out.str();
    }

}