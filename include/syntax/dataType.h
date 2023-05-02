#ifndef SYNTAX_DATATYPE_H_
#define SYNTAX_DATATYPE_H_

#include <iostream>
#include <string>
#include <map>
#include <variant>
#include <vector>
#include "functionGroup.h"

namespace Snapp {

    class FunctionGroup;

    using IntValue = int;
    using FloatValue = double;
    using BoolValue = bool;
    using StrValue = std::string;

    struct FunctionValue;
    struct NativeFunctionValue;

    using DataValue = std::variant<IntValue, FloatValue, BoolValue, StrValue, FunctionValue, NativeFunctionValue, FunctionGroup>;

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
        DataType(BaseDataType base, std::string name, bool live);

        BaseDataType base() const;
        const std::string& name() const;
        bool live() const;
        static DataType getDataType(const DataValue& value);


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
        std::function<DataValue(std::vector<DataValue>)> body;
    };
}

#endif /* SYNTAX_DATATYPE_H_ */