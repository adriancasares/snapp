#ifndef SNAPP_VALUE_CLASS_H_
#define SNAPP_VALUE_CLASS_H_

#include "dataType.h"
#include "function.h"

#include <vector>

namespace Snapp {

    class Scope;

    class ClassValue {
    public:
        ClassValue();

        FunctionValue& constructor();
        Scope* scope();
        void setScope(Scope* scope);

    private:
        FunctionValue constructor_;
        Scope* scope_;
    };

}

#endif /* SNAPP_VALUE_CLASS_H_ */
