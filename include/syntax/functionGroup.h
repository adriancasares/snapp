//
// Created by Adrian Casares on 5/2/23.
//

#ifndef SNAPP_INCLUDE_SYNTAX_FUNCTIONGROUP_H_
#define SNAPP_INCLUDE_SYNTAX_FUNCTIONGROUP_H_

#include "dataType.h"

namespace Snapp {

  class DataType;
  class FunctionValue;
  class NativeFunctionValue;

  class FunctionGroup {
    public:
      FunctionGroup();
        void addFunction(const FunctionValue &function);
        void addFunction(const NativeFunctionValue& function);
        std::vector<std::variant<FunctionValue, NativeFunctionValue>>& functions();
        const std::vector<std::variant<FunctionValue, NativeFunctionValue>>& functions() const;
        bool hasFunction(const std::vector<DataType>& parameters) const;
        std::variant<FunctionValue, NativeFunctionValue>& getFunction(const std::vector<DataType>& parameters);
    private:
      std::vector<std::variant<FunctionValue, NativeFunctionValue>> functions_;
  };
}
#endif //SNAPP_INCLUDE_SYNTAX_FUNCTIONGROUP_H_
