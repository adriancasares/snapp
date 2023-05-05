#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"

namespace Snapp::Native {

    FunctionValue createPrintFunction() {
        FunctionValue printFunction;

        printFunction.addOverload({
                                          DataType::Void,
                                          {DataType::Str},
                                          [](const std::vector<DataValue>& args) {
                                              std::cout << *coerceStr(args[0]) << std::endl;
                                              return std::nullopt;
                                          }
                                  });

        printFunction.addOverload({
                                          DataType::Void,
                                          {DataType::Int},
                                          [](const std::vector<DataValue>& args) {
                                              std::cout << *coerceInt(args[0]) << std::endl;
                                              return std::nullopt;
                                          }
                                  });

        printFunction.addOverload({
                                          DataType::Void,
                                          {DataType::Float},
                                          [](const std::vector<DataValue>& args) {
                                              std::cout << *coerceFloat(args[0]) << std::endl;
                                              return std::nullopt;
                                          }
                                  });

        printFunction.addOverload({
                                          DataType::Void,
                                          {DataType::Bool},
                                          [](const std::vector<DataValue>& args) {
                                              std::cout << (*coerceBool(args[0]) ? "true" : "false") << std::endl;
                                              return std::nullopt;
                                          }
                                  });

        return printFunction;
    }

    FunctionValue createInputFunction() {
        FunctionValue inputFunction;

        inputFunction.addOverload({
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