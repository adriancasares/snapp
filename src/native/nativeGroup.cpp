#include "native/nativeGroup.h"
#include "value/class.h"
#include "native/root/rootArray.h"
#include "native/root/rootIO.h"
#include "native/root/rootString.h"


namespace Snapp::Native {

    void injectNativeGroup(Scope* scope) {
        scope->add("io", createRootIO());
        scope->add("Array", createRootArray());
        scope->add("string", createRootString());
    }

}