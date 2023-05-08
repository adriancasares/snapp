#ifndef SNAPP_SEMANTICS_SCOPE_H_
#define SNAPP_SEMANTICS_SCOPE_H_

#include "value/dataValue.h"
#include "value/function.h"

#include <map>
#include <string>
#include "value/class.h"
namespace Snapp {

    class Scope {
    public:
        Scope(Scope* parent, bool strong, bool isFunction = false, ClassValue* classValue = nullptr);

        bool isFunction() const;
        bool hasParent() const;
        bool isClass() const;
        ClassValue* getClass() const;
        bool isStrong() const;
        Scope* parent() const;
        DataValue& get(const std::string& name);
        bool has(const std::string& name) const;
        void add(const std::string& name, const DataValue& value);
        void assign(const std::string& name, const DataValue& value);

        void setParent(Scope* parent);

        std::map<std::string, DataValue> identifiers_;
     private:
        Scope* parent_;
        bool strong_;
        ClassValue* class_;
        bool isFunction_;
    };

}

#endif /* SNAPP_SEMANTICS_SCOPE_H_ */