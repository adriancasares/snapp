#ifndef SNAPP_VALUE_DATAVALUE_H_
#define SNAPP_VALUE_DATAVALUE_H_

#include "dataType.h"
#include "primitive.h"
#include "class.h"

#include <variant>

namespace Snapp {

    struct FunctionValue;
    struct NativeFunctionValue;
    class FunctionGroup;

    using AnyFunction = std::variant<FunctionValue, NativeFunctionValue, FunctionGroup>;
    using DataValue = std::variant<IntValue, FloatValue, BoolValue, StrValue, AnyFunction, ClassValue>;

    DataType dataTypeOf(const DataValue& value);

}

#endif /* SNAPP_VALUE_DATAVALUE_H_ */
