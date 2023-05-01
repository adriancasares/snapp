//
// Created by Adrian Casares on 5/1/23.
//
#include "native/root/RootIO.h"
#include "semantics/scope.h"
#include "syntax/dataType.h"
namespace Snapp {

    Snapp::NativeFunctionValue Snapp::createPrintFunction() {
      std::vector<DataType> parameters = {DataType(BaseDataType::Str, "str", false)};

      NativeFunctionValue printFunction = {
            DataType(BaseDataType::Void, "void", false),
            parameters,
            [](const std::vector<DataValue>& args) {
                std::cout << std::get<StrValue>(args[0]) << std::endl;
                return DataValue();
            }
      };

      return printFunction;
    }

    Snapp::NativeFunctionValue Snapp::createInputFunction() {
      std::vector<DataType> parameters = {DataType(BaseDataType::Str, "str", false)};

      NativeFunctionValue inputFunction = {
            DataType(BaseDataType::Str, "str", false),
            parameters,
            [](const std::vector<DataValue>& args) {
                std::cout << std::get<StrValue>(args[0]);
                std::string input;
                std::cin >> input;
                return DataValue(input);
            }
      };

      return inputFunction;
    }

}