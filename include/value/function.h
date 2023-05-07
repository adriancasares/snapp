#ifndef SNAPP_VALUE_FUNCTION_H_
#define SNAPP_VALUE_FUNCTION_H_

#include "dataType.h"
#include "dataValue.h"

#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace Snapp {

    class SyntaxNode;

    struct InterpretedFunction {
        struct Parameter {
            DataType type;
            std::string name;
        };

        DataType returnType;
        std::vector<Parameter> parameters;
        SyntaxNode* body;
        bool isConstructor;
    };

    struct NativeFunction {
        DataType returnType;
        std::vector<DataType> parameters;
        std::function<std::optional<DataValue>(const std::vector<DataValue>&)> body;
    };

    using FunctionOverload = std::variant<InterpretedFunction, NativeFunction>;

    class FunctionValue {
    public:
        FunctionValue() = default;

        std::vector<FunctionOverload>& overloads();
        const std::vector<FunctionOverload>& overloads() const;

        const FunctionOverload* getOverload(const std::vector<DataType>& parameters) const;

        void addOverload(const InterpretedFunction& function);
        void addOverload(const NativeFunction& function);
        void addOverload(const FunctionOverload& function);

    private:
        std::vector<FunctionOverload> overloads_;
    };

}

#endif /* SNAPP_VALUE_FUNCTION_H_ */
