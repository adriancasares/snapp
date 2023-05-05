#include "value/class.h"
# include "value/dataValue.h"

namespace Snapp {

    ClassValue::ClassValue() {
        scope_ = nullptr;
    }

    FunctionValue& ClassValue::constructor() {
        return constructor_;
    }

    Scope* ClassValue::scope() {
        return scope_;
    }

    void ClassValue::setScope(Scope* scope) {
        scope_ = scope;
    }

}