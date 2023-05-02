#ifndef SYNTAX_DATATYPE_H_
#define SYNTAX_DATATYPE_H_

#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <vector>
#include <functional>

namespace Snapp {

    using IntValue = int;
    using FloatValue = double;
    using BoolValue = bool;
    using StrValue = std::string;

    struct FunctionValue;
    struct NativeFunctionValue;

    using DataValue = std::variant<IntValue, FloatValue, BoolValue, StrValue, FunctionValue, NativeFunctionValue>;

    enum class BaseDataType {
        Unknown,
        Void,
        Int,
        Float,
        Bool,
        Str,
        Function,
        Object,
    };

    const std::map<std::string, BaseDataType> nameToBaseDataType = {
        {"void", BaseDataType::Void},
        {"int", BaseDataType::Int},
        {"float", BaseDataType::Float},
        {"bool", BaseDataType::Bool},
        {"str", BaseDataType::Str},
        {"Object", BaseDataType::Object},
    };

    BaseDataType findBaseDataType(const std::string& name);

    class DataType {
    public:
        DataType(BaseDataType base, std::string name, bool live = false);

        BaseDataType base() const;
        const std::string& name() const;
        bool live() const;

        static DataType Void;
        static DataType Int;
        static DataType Float;
        static DataType Bool;
        static DataType Str;

    private:
        BaseDataType base_;
        std::string name_;
        bool live_;
    };

    std::ostream& operator<<(std::ostream& out, const DataType& dataType);

    class SyntaxNode;

    struct FunctionValue {
        struct Parameter {
            DataType type;
            std::string name;
        };

        DataType returnType;
        std::vector<Parameter> parameters;
        SyntaxNode* body;
    };

    struct NativeFunctionValue {
        DataType returnType;
        std::vector<DataType> parameters;
        std::function<std::optional<DataValue>(const std::vector<DataValue>&)> body;
    };

}

#endif /* SYNTAX_DATATYPE_H_ */