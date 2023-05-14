#include "value/import.h"
#include "semantics/scope.h"

namespace Snapp {

    ImportValue::ImportValue(Scope* scope) {
        scope_ = scope;
    }

    const Scope* ImportValue::scope() const {
        return scope_;
    }

    Scope* ImportValue::scope() {
        return scope_;
    }

    void ImportValue::setScope(Scope* scope) {
        scope_ = scope;
    }

}