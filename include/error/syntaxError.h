#ifndef ERROR_SYNTAXERROR_H_
#define ERROR_SYNTAXERROR_H_

#include <string>
#include <optional>

#include "syntax/sourceLocation.h"

namespace Snapp {

    class SyntaxError {
    public:
        SyntaxError();
        SyntaxError(std::string message);
        SyntaxError(std::string message, std::optional<SourceLocation> start, std::optional<SourceLocation> end);
    
        const std::string& message() const;
        const std::optional<SourceLocation>& start() const;
        const std::optional<SourceLocation>& end() const;
    
        std::string output(const std::string& sourceCode) const;
    
    private:
        std::string message_;
        std::optional<SourceLocation> start_, end_;
    };

}

#endif /* ERROR_SYNTAXERROR_H_ */
