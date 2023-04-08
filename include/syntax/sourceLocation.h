//
// Created by Adrian Casares on 4/7/23.
//

#ifndef SNAPP_INCLUDE_SOURCELOCATION_H_
#define SNAPP_INCLUDE_SOURCELOCATION_H_

#include <iostream>

namespace Snapp {
  struct SourceLocation {
    int index = 0;
    int line = 1;
    int column = 1;
  };

  std::ostream& operator<<(std::ostream& out, const SourceLocation& location);
}
#endif //SNAPP_INCLUDE_SOURCELOCATION_H_
