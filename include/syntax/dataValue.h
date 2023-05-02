//
// Created by Adrian Casares on 5/2/23.
//

#ifndef SNAPP_INCLUDE_SYNTAX_DATAVALUE_H_
#define SNAPP_INCLUDE_SYNTAX_DATAVALUE_H_

#include "dataType.h"

#include "values/classValue.h"
#include "values/primitiveValues.h"

namespace Snapp {
  using DataValue = std::variant<IntValue, FloatValue, BoolValue, StrValue, FunctionValue, NativeFunctionValue, FunctionGroup, ClassValue>;
}
#endif //SNAPP_INCLUDE_SYNTAX_DATAVALUE_H_
