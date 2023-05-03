#ifndef SNAPP_SYNTAX_SOURCELOCATION_H_
#define SNAPP_SYNTAX_SOURCELOCATION_H_

#include <iostream>

namespace Snapp {

    struct SourceLocation {
        int index = 0;
        int line = 1;
        int column = 1;
    };

    std::ostream& operator<<(std::ostream& out, const SourceLocation& location);

}

#endif /* SNAPP_SYNTAX_SOURCELOCATION_H_ */
