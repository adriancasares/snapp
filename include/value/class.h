#ifndef SNAPP_VALUE_CLASS_H_
#define SNAPP_VALUE_CLASS_H_

#include "dataType.h"
#include "function.h"

#include <vector>

namespace Snapp {

    class Scope;
    class SyntaxNode;

    class ClassValue {
    public:
        ClassValue() = default;

        Scope* scope() const;
        const std::vector<FunctionValue>& constructors() const;

        void addConstructor(const FunctionValue& constructor);
        void setScope(Scope* scope);

        bool hasConstructor(const std::vector<DataType>& arguments) const;
        FunctionValue* getConstructor(const std::vector<DataType>& arguments);

    private:
        Scope* scope_;
        std::vector<FunctionValue> constructors_;
    };

}

#endif /* SNAPP_VALUE_CLASS_H_ */
