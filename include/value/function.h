#ifndef SNAPP_VALUE_FUNCTION_H_
#define SNAPP_VALUE_FUNCTION_H_

#include "dataType.h"
#include "dataValue.h"
#include "function.h"
#include "class.h"

#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace Snapp {
//
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

    using SimpleFunctionValue = std::variant<FunctionValue, NativeFunctionValue>;

    class FunctionGroup {
    public:
        FunctionGroup();

        std::vector<SimpleFunctionValue>& functions();
        const std::vector<SimpleFunctionValue>& functions() const;

        bool hasFunction(const std::vector<DataType>& parameters) const;
        SimpleFunctionValue& getFunction(const std::vector<DataType>& parameters);

        void addFunction(const FunctionValue& function);
        void addFunction(const NativeFunctionValue& function);

    private:
        std::vector<SimpleFunctionValue> functions_;
    };

}

#endif /* SNAPP_VALUE_FUNCTION_H_ */
