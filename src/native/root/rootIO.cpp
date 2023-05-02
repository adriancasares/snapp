//
// Created by Adrian Casares on 5/1/23.
//

#include "native/root/rootIO.h"
#include "syntax/dataType.h"
#include "semantics/astRunner.h"

namespace Snapp::Native {

    FunctionGroup createPrintFunction() {

      FunctionGroup printFunction;

      printFunction.addFunction({
          DataType::Void,
          {DataType::Str},
          [](const std::vector<DataValue>& args) {
              std::cout << *coerceStr(args[0]) << std::endl;
              return std::nullopt;
          }
      });

      printFunction.addFunction({
        DataType::Void,
        { DataType::Int },
        [](const std::vector<DataValue>& args) {
          std::cout << *coerceInt(args[0]) << std::endl;
          return std::nullopt;
        }
      });

      printFunction.addFunction({
        DataType::Void,
        { DataType::Float },
        [](const std::vector<DataValue>& args) {
          std::cout << *coerceFloat(args[0]) << std::endl;
          return std::nullopt;
        }
      });

      printFunction.addFunction({
          DataType::Void,
          { DataType::Bool },
          [](const std::vector<DataValue>& args) {
          std::cout << (*coerceBool(args[0]) ? "true" : "false") << std::endl;
          return std::nullopt;
          }
      });

      return printFunction;
    }

    FunctionGroup createInputFunction() {
        FunctionGroup inputFunction;

        inputFunction.addFunction({
            DataType::Str,
            {DataType::Str},
            [](const std::vector<DataValue>& args) {
                std::cout << *coerceStr(args[0]);
                std::string input;
                std::cin.ignore(); // unclear exactly what this fixes, but getline() doesn't work otherwise
                std::getline(std::cin, input);
                return input;
            }
        });

        return inputFunction;
    }

}