#include "native/nativeGroup.h"
#include "value/class.h"
#include "native/root/rootArray.h"
#include "native/root/rootIO.h"


namespace Snapp::Native {

    void injectNativeGroup(Scope* scope) {
        scope->add("io", createRootIO());
        scope->add("Array", createRootArray());
    }

}