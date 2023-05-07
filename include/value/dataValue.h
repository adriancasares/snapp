#ifndef SNAPP_VALUE_DATAVALUE_H_
#define SNAPP_VALUE_DATAVALUE_H_

#include "primitive.h"

#include <optional>
#include <variant>

namespace Snapp {

    class FunctionValue;
    class ObjectValue;
    class ClassValue;
    class DataType;
    class ImportValue;

    using DataValue = std::variant<IntValue, FloatValue, BoolValue, StrValue, FunctionValue, ClassValue*, ObjectValue*, ImportValue*>;

    DataType dataTypeOf(const DataValue& value);

    std::optional<IntValue> coerceInt(const DataValue& value);
    std::optional<IntValue> convertInt(const DataValue& value);
    std::optional<FloatValue> coerceFloat(const DataValue& value);
    std::optional<FloatValue> convertFloat(const DataValue& value);
    std::optional<BoolValue> coerceBool(const DataValue& value);
    std::optional<BoolValue> convertBool(const DataValue& value);
    std::optional<StrValue> coerceStr(const DataValue& value);
    std::optional<StrValue> convertStr(const DataValue& value);

}

#endif /* SNAPP_VALUE_DATAVALUE_H_ */
