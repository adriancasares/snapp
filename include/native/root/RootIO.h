//
// Created by Adrian Casares on 5/1/23.
//

#ifndef SNAPP_INCLUDE_NATIVE_ROOT_ROOTIO_H_
#define SNAPP_INCLUDE_NATIVE_ROOT_ROOTIO_H_

#include "semantics/scope.h"
namespace Snapp {

    NativeFunctionValue createPrintFunction();

    NativeFunctionValue createInputFunction();

}
#endif //SNAPP_INCLUDE_NATIVE_ROOT_ROOTIO_H_
