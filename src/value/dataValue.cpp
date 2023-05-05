#include "value/dataValue.h"
#include "value/function.h"
#include "value/class.h"

namespace Snapp {

    DataType dataTypeOf(const DataValue& value) {
        if (std::holds_alternative<IntValue>(value)) {
            return {BaseDataType::Int, "int", false};
        }
        if (std::holds_alternative<FloatValue>(value)) {
            return {BaseDataType::Float, "float", false};
        }
        if (std::holds_alternative<BoolValue>(value)) {
            return {BaseDataType::Bool, "bool", false};
        }
        if (std::holds_alternative<StrValue>(value)) {
            return {BaseDataType::Str, "str", false};
        }
        if (std::holds_alternative<FunctionValue>(value)) {
            return {BaseDataType::Function, "Function", false};
        }
        if (std::holds_alternative<ClassValue>(value)) {
            return {BaseDataType::Class, "Class", false};
        }
        return {BaseDataType::Unknown, "Unknown", false};
    }

    std::optional<IntValue> coerceInt(const DataValue& value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x;
        }
        return {};
    }

    std::optional<IntValue> convertInt(const DataValue& value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<FloatValue>(&value)) {
            return (IntValue) *x;
        }
        if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x;
        }
        return {};
    }

    std::optional<FloatValue> coerceFloat(const DataValue& value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<FloatValue>(&value)) {
            return *x;
        }
        return {};
    }

    std::optional<FloatValue> convertFloat(const DataValue& value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<FloatValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x ? 1.0 : 0.0;
        }
        return {};
    }

    std::optional<BoolValue> coerceBool(const DataValue& value) {
        if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x;
        }
        return {};
    }

    std::optional<BoolValue> convertBool(const DataValue& value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<FloatValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<StrValue>(&value)) {
            return !x->empty();
        }
        return {};
    }

    std::optional<StrValue> coerceStr(const DataValue& value) {
        if (auto* x = std::get_if<StrValue>(&value)) {
            return *x;
        }
        return {};
    }

    std::optional<StrValue> convertStr(const DataValue& value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return std::to_string(*x);
        }
        if (auto* x = std::get_if<FloatValue>(&value)) {
            return std::to_string(*x);
        }
        if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x ? "true" : "false";
        }
        if (auto* x = std::get_if<StrValue>(&value)) {
            return *x;
        }
        return {};
    }

}