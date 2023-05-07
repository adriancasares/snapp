//
// Created by Adrian Casares on 5/6/23.
//

#include "value/object.h"
#include "value/class.h"
#include "semantics/scope.h"
#include "semantics/astRunner.h"

namespace Snapp {

    class ASTRunner;

    ObjectValue::ObjectValue(ClassValue* classValue) {
        classValue_ = classValue;
        Scope* scope = new Scope(nullptr, true);
        scope_ = scope;
    }

    const Scope* ObjectValue::scope() const {
        return scope_;
    }

    Scope* ObjectValue::scope() {
        return scope_;
    }

    void ObjectValue::setScope(Scope* scope) {
        scope_ = scope;
    }

    ClassValue* ObjectValue::classValue() {
        return classValue_;
    }

    const ClassValue* ObjectValue::classValue() const {
        return classValue_;
    }
}