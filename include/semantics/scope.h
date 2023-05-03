#ifndef SNAPP_SEMANTICS_SCOPE_H_
#define SNAPP_SEMANTICS_SCOPE_H_

#include "value/dataValue.h"
#include "value/function.h"

#include <map>
#include <string>

namespace Snapp {

    class Scope {
    public:
        Scope(Scope* parent = nullptr, bool isFunction = false, ClassValue* classValue = nullptr);

        bool isFunction() const;
        bool hasParent() const;
        bool isClass() const;

        DataValue& get(const std::string& name);
        bool has(const std::string& name) const;
        void add(const std::string& name, const DataValue& value);
        void assign(const std::string& name, const DataValue& value);

    private:
        Scope* parent_;
        ClassValue* class_;
        bool isFunction_;
        std::map<std::string, DataValue> identifiers_;
    };

}

#endif /* SNAPP_SEMANTICS_SCOPE_H_ */