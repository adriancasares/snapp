//
// Created by Adrian Casares on 5/6/23.
//

#include "value/import.h"
#include "semantics/scope.h"
#include "semantics/astRunner.h"

namespace Snapp {

    class ASTRunner;

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