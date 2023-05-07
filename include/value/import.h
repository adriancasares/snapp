#ifndef SNAPP_VALUE_IMPORT_H_
#define SNAPP_VALUE_IMPORT_H_

#include "dataType.h"
#include "function.h"
#include "error/runtimeError.h"
#include "semantics/astRunner.h"
#include <vector>

namespace Snapp {

    class Scope;

    class ImportValue {
      public:
          ImportValue(Scope* scope);
          const Scope* scope() const;
          Scope* scope();
          void setScope(Scope* scope);
       private:
          Scope* scope_;
    };

}

#endif /* SNAPP_VALUE_IMPORT_H_ */
