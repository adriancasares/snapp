#include "semantics/scope.h"
#include "error/runtimeError.h"

namespace Snapp {

    Scope::Scope(Scope* parent, bool isFunction, ClassValue* classValue) {
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
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            return true;
        } else if (parent_) {
            return parent_->has(name);
        }
        return false;
    }

    DataValue& Scope::get(const std::string& name) {
        if (isClass()) {
            return class_->scope()->get(name);
        }

        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            return it->second;
        } else if (parent_) {
            return parent_->get(name);
        } else {
            throw RuntimeError("undefined identifier '" + name + "'");
        }
    }

    void Scope::assign(const std::string &name, const Snapp::DataValue &value) {
        if (isClass()) {
            class_->scope()->assign(name, value);
            return;
        }

        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            it->second = value;
        } else if (parent_) {
            parent_->assign(name, value);
        } else {
            throw RuntimeError("undefined identifier '" + name + "'");
        }
    }

    void Scope::add(const std::string& name, const DataValue& value) {
        if (isClass()) {
            class_->scope()->add(name, value);
            return;
        }

        if (has(name)) {
            assign(name, value);
        }

        identifiers_.insert({name, value});
    }

}