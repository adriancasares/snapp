#include "value/class.h"

namespace Snapp {

    Scope* ClassValue::scope() const {
        return scope_;
    }

    const std::vector<FunctionValue>& ClassValue::constructors() const {
        return constructors_;
    }

    void ClassValue::addConstructor(const FunctionValue& constructor) {
        constructors_.push_back(constructor);
    }

    void ClassValue::setScope(Scope* scope) {
        scope_ = scope;
    }

}