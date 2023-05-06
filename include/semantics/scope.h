#ifndef SNAPP_SEMANTICS_SCOPE_H_
#define SNAPP_SEMANTICS_SCOPE_H_

#include "value/dataValue.h"
#include "value/function.h"

#include <map>
#include <string>

namespace Snapp {

    class Scope {
    public:
        Scope(Scope* parent, bool strong, bool isFunction = false, bool isClass = true);

        bool isFunction() const;
        bool hasParent() const;
        bool isClass() const;
        bool isStrong() const;
        Scope* parent() const;

        DataValue& get(const std::string& name);
        bool has(const std::string& name) const;
        void add(const std::string& name, const DataValue& value);
        void assign(const std::string& name, const DataValue& value);

    private:
        Scope* parent_;
        bool strong_;
        bool isClass_;
        bool isFunction_;
        std::map<std::string, DataValue> identifiers_;
    };

}

#endif /* SNAPP_SEMANTICS_SCOPE_H_ */