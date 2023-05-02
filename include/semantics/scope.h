#ifndef SEMANTICS_SCOPE_H_
#define SEMANTICS_SCOPE_H_

#include "syntax/values/classValue.h"
#include "syntax/dataType.h"
#include "syntax/dataValue.h"

namespace Snapp {

    class ClassValue;

    class Scope {
    public:
        Scope(Scope* parent = nullptr, bool isFunction = false, ClassValue* classValue = nullptr);

        bool isFunction() const;
        bool hasParent() const;
        bool isClass() const;

        DataValue& get(const std::string& name);
        bool exists(const std::string& name) const;
        void add(const std::string& name, const DataValue& value);
        void assign(const std::string& name, const DataValue& value);

    private:
        Scope* parent_;
        ClassValue* class_;
        bool isFunction_;
        std::map<std::string, DataValue> identifiers_;
    };

}

#endif /* SEMANTICS_SCOPE_H_ */