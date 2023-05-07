#ifndef SNAPP_VALUE_OBJECT_H_
#define SNAPP_VALUE_OBJECT_H_

#include "dataType.h"
#include "function.h"
#include "error/runtimeError.h"
#include "semantics/astRunner.h"
#include <vector>

namespace Snapp {

    class Scope;

    class ObjectValue {
      public:
          ObjectValue(ClassValue* classValue);
          const Scope* scope() const;
          Scope* scope();
          void setScope(Scope* scope);
          const ClassValue* classValue() const;
          ClassValue* classValue();
       private:
          Scope* scope_;
          ClassValue* classValue_;
    };

}

#endif /* SNAPP_VALUE_OBJECT_H_ */
