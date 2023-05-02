//
// Created by Adrian Casares on 5/1/23.
//

#include "semantics/scope.h"
#include "native/nativeGroup.h"
#include "native/root/rootIO.h"

namespace Snapp {

    void initializeNativeGroup(Scope* scope) {
        scope->add("print", createPrintFunction());
        scope->add("input", createInputFunction());
    }

}