#include "value/class.h"
# include "value/dataValue.h"

namespace Snapp {

    ClassValue::ClassValue(std::string name) {
        name_ = name;
    }

    FunctionValue& ClassValue::constructor() {
        return constructor_;
    }

    bool ClassValue::has(const std::string& name) const {
      return (identifiers_.find(name) != identifiers_.end());
    }

    ClassIdentifierValue& ClassValue::get(const std::string& name) {
      if (auto it = identifiers_.find(name); it != identifiers_.end()) {
        return it->second;
      } else {
        throw RuntimeError("undefined identifier '" + name + "'");
      }
    }

    void ClassValue::assign(const std::string &name, const ClassIdentifierValue &value) {
      if (auto it = identifiers_.find(name); it != identifiers_.end()) {
        it->second = value;
      } else {
        throw RuntimeError("undefined identifier '" + name + "'");
      }
    }

    void ClassValue::add(const std::string& name, const ClassIdentifierValue& value) {
      if (has(name)) {
        assign(name, value);
      }
      identifiers_.insert_or_assign(name, value);
    }

    std::string ClassValue::name() const {
        return name_;
    }

    std::map<std::string, ClassIdentifierValue>& ClassValue::identifiers() {
        return identifiers_;
    }

    const std::map<std::string, ClassIdentifierValue>& ClassValue::identifiers() const {
        return identifiers_;
    }

    void ClassValue::setScope(Scope* scope) {
        scope_ = scope;
    }

    Scope* ClassValue::scope() const {
        return scope_;
    }
}