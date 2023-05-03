#ifndef SNAPP_VALUE_CLASS_H_
#define SNAPP_VALUE_CLASS_H_

#include "dataType.h"

#include <vector>

namespace Snapp {

    class Scope;
    class SyntaxNode;

    // TODO: constructor could just be a FunctionGroup where return type is void
    // also: methods n stuff
    struct ConstructorValue {
        std::vector<DataType> parameters;
        SyntaxNode* body;
    };

    class ClassValue {
    public:
        ClassValue() = default;

        Scope* scope() const;
        const std::vector<ConstructorValue>& constructors() const;

        void addConstructor(const ConstructorValue& constructor);
        void setScope(Scope* scope);

    private:
        Scope* scope_;
        std::vector<ConstructorValue> constructors_;
    };

}

#endif /* SNAPP_VALUE_CLASS_H_ */
