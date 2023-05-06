#include "value/class.h"
# include "value/dataValue.h"

namespace Snapp {

    ClassValue::ClassValue() {
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
}