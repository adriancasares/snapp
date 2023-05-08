#include "native/nativeGroup.h"
#include "native/root/rootIO.h"
#include "native/root/rootExternal.h"
#include "value/class.h"

namespace Snapp::Native {

    void injectNativeGroup(Scope* scope) {
        scope->add("print", createPrintFunction());
        scope->add("input", createInputFunction());
        scope->add("use_external_function", createUseExternalFunction());
    }

}