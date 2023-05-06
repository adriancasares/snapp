#ifndef SNAPP_VALUE_CLASS_H_
#define SNAPP_VALUE_CLASS_H_

#include "dataType.h"
#include "function.h"
#include "error/runtimeError.h"
#include <vector>

namespace Snapp {

    class Scope;

    struct ClassAttribute {
        DataType type;
        SyntaxNode* initializer;
    };
    using ClassIdentifierValue = std::variant<ClassAttribute, FunctionValue>;

    class ClassValue {
    public:
        ClassValue(std::string name);
        FunctionValue& constructor();

        ClassIdentifierValue& get(const std::string& name);
        std::string name() const;
        bool has(const std::string& name) const;
        void add(const std::string& name, const ClassIdentifierValue& value);
        void assign(const std::string& name, const ClassIdentifierValue& value);


     private:
        std::string name_;
        FunctionValue constructor_;
        std::map<std::string, ClassIdentifierValue> identifiers_;

    };

}

#endif /* SNAPP_VALUE_CLASS_H_ */
