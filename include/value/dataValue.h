#ifndef SNAPP_VALUE_DATAVALUE_H_
#define SNAPP_VALUE_DATAVALUE_H_
#include <variant>

#include "primitive.h"

namespace Snapp {

    struct FunctionValue;
    struct NativeFunctionValue;
    class FunctionGroup;
    class ClassValue;
    class DataType;


    using AnyFunction = std::variant<FunctionValue, NativeFunctionValue, FunctionGroup>;
    using DataValue = std::variant<IntValue, FloatValue, BoolValue, StrValue, AnyFunction, ClassValue>;

    DataType dataTypeOf(const DataValue& value);

}

#endif /* SNAPP_VALUE_DATAVALUE_H_ */
