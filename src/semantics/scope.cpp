#include "semantics/scope.h"

namespace Snapp {

    Scope::Scope(Scope* parent, bool isFunction) {
        parent_ = parent;
        isFunction_ = isFunction;
    }

    bool Scope::isFunction() const {
        return isFunction_;
    }

    bool Scope::hasParent() const {
        return parent_ != nullptr;
    }

    bool Scope::exists(const std::string& name) const {
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            return true;
        } else if (parent_) {
            return parent_->exists(name);
        }
        return false;
    }

    DataValue& Scope::get(const std::string& name) {
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            return it->second;
        } else /*if (parent_)*/ { // uncomment when error is implemented
            return parent_->get(name);
        }
        // TODO error
    }

    void Scope::assign(const std::string &name, const Snapp::DataValue &value) {
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            it->second = value;
        } else /*if (parent_)*/ { // uncomment when error is implemented
            parent_->assign(name, value);
        }
    }

    void Scope::add(const std::string& name, const DataValue& value) {
        identifiers_.insert_or_assign(name, value);
    }

}