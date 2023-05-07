#include "semantics/scope.h"
#include "value/class.h"
#include "error/runtimeError.h"

namespace Snapp {

    Scope::Scope(Scope* parent, bool strong, bool isFunction, ClassValue* classValue) {
        identifiers_ = std::map<std::string, DataValue>();
        strong_ = strong;
        parent_ = parent;
        class_ = classValue;
        isFunction_ = isFunction;
    }

    bool Scope::isFunction() const {
        return isFunction_;
    }

    bool Scope::hasParent() const {
        return parent_ != nullptr;
    }

    bool Scope::isClass() const {
        return class_ != nullptr;
    }

    bool Scope::has(const std::string& name) const {
        if (identifiers_.find(name) != identifiers_.end()) {
            return true;
        } else if (parent_) {
            return parent_->has(name);
        }
        return false;
    }

    DataValue& Scope::get(const std::string& name) {
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            return it->second;
        } else if (parent_) {
            return parent_->get(name);
        } else {
            throw RuntimeError("undefined identifier '" + name + "'");
        }
    }

    void Scope::assign(const std::string &name, const DataValue &value) {
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            it->second = value;
        } else if (parent_) {
            parent_->assign(name, value);
        } else {
            throw RuntimeError("undefined identifier '" + name + "'");
        }
    }

    void Scope::add(const std::string& name, const DataValue& value) {
        if (has(name)) {
            assign(name, value);
        }
        identifiers_.insert_or_assign(name, value);
    }

    Scope* Scope::parent() const {
        return parent_;
    }

    ClassValue* Scope::getClass() const {
        return class_;
    }

    bool Scope::isStrong() const {
      return strong_;
    }

    void Scope::setParent(Scope* parent) {
        parent_ = parent;
    }

}