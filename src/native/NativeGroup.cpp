//
// Created by Adrian Casares on 5/1/23.
//

#include "semantics/scope.h"
#include "native/NativeGroup.h"
#include "native/root/RootIO.h"

namespace Snapp {

  void Snapp::initializeNativeGroup(Snapp::Scope *scope) {

    scope->add("print", createPrintFunction());
    scope->add("input", createInputFunction());
  }

}