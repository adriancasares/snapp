#include "value/dataValue.h"
#include "value/function.h"

namespace Snapp {

    DataType dataTypeOf(const DataValue& value) {
        if (std::holds_alternative<IntValue>(value)) {
            return DataType(BaseDataType::Int, "int", false);
        } else if (std::holds_alternative<FloatValue>(value)) {
            return DataType(BaseDataType::Float, "float", false);
        } else if (std::holds_alternative<BoolValue>(value)) {
            return DataType(BaseDataType::Bool, "bool", false);
        } else if (std::holds_alternative<StrValue>(value)) {
            return DataType(BaseDataType::Str, "str", false);
        } else if (std::holds_alternative<AnyFunction>(value)) {
            return DataType(BaseDataType::Function, "Function", false);
        } else if (std::holds_alternative<ClassValue>(value)) {
            return DataType(BaseDataType::Class, "Class", false);
        } else {
            return DataType(BaseDataType::Unknown, "Unknown", false);
        }
    }

}