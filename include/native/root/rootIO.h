#ifndef SNAPP_NATIVE_ROOT_ROOTIO_H_
#define SNAPP_NATIVE_ROOT_ROOTIO_H_

#include "semantics/scope.h"
#include "value/function.h"

namespace Snapp::Native {

    FunctionValue createPrintFunction();
    FunctionValue createInputFunction();

}

#endif /* SNAPP_NATIVE_ROOT_ROOTIO_H_ */
