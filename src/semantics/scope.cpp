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
    
    DataValue& Scope::get(const std::string& name) {
        if (auto it = identifiers_.find(name); it != identifiers_.end()) {
            return it->second;
        } else /*if (parent_)*/ { // uncomment when error is implemented
            return parent_->get(name);
        }
        // TODO error
    }

    void Scope::add(const std::string& name, const DataValue& value) {
        identifiers_.insert_or_assign(name, value);
    }

}