//
// Created by Adrian Casares on 5/1/23.
//

#ifndef SNAPP_NATIVE_ROOT_ROOTIO_H_
#define SNAPP_NATIVE_ROOT_ROOTIO_H_

#include "semantics/scope.h"
#include "syntax/functionGroup.h"

namespace Snapp::Native {

    FunctionGroup createPrintFunction();
    FunctionGroup createInputFunction();

}

#endif /* SNAPP_NATIVE_ROOT_ROOTIO_H_ */
