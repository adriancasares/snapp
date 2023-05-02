//
// Created by Adrian Casares on 5/2/23.
//

#ifndef SNAPP_INCLUDE_SYNTAX_CLASSVALUE_H_
#define SNAPP_INCLUDE_SYNTAX_CLASSVALUE_H_

#include "semantics/scope.h"
#include <vector>

namespace Snapp {
  class Scope;
  class ConstructorValue;

  class ClassValue {
   private:
    Scope* scope_;
    std::vector<ConstructorValue> constructors_ = {};
   public:
    ClassValue() = default;
    Scope* scope() const;
    void addConstructor(const ConstructorValue& constructor);
    const std::vector<ConstructorValue>& constructors() const;
    void setScope(Scope* scope);
  };
}
#endif //SNAPP_INCLUDE_SYNTAX_CLASSVALUE_H_
