#include "syntax/sourceLocation.h"

namespace Snapp {

    std::ostream& operator<<(std::ostream& out, const SourceLocation& location) {
        return out << "line " << location.line << ":" << location.column;
    }

}