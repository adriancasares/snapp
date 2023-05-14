#include "native/root/rootRandom.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "value/import.h"

namespace Snapp::Native {


    ImportValue* createRootRandom() {
        auto scope = new Scope(nullptr, true);
        auto rootRandom = new ImportValue(scope);

        // make this function as well as others
        //scope->add("randomFloat", createRandomFloatFunction());

        return rootRandom;
    }

}