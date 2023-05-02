#ifndef SEMANTICS_SCOPE_H_
#define SEMANTICS_SCOPE_H_

#include "syntax/dataType.h"

namespace Snapp {

    class Scope {
    public:
        Scope(Scope* parent = nullptr, bool isFunction = false);

        bool isFunction() const;
        bool hasParent() const;

        DataValue& get(const std::string& name);
        bool exists(const std::string& name) const;
        void add(const std::string& name, const DataValue& value);
        void assign(const std::string& name, const DataValue& value);

    private:
        Scope* parent_;
        bool isFunction_;
        std::map<std::string, DataValue> identifiers_;
    };

}

#endif /* SEMANTICS_SCOPE_H_ */