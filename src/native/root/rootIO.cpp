//
// Created by Adrian Casares on 5/1/23.
//

#include "native/root/rootIO.h"
#include "syntax/dataType.h"
#include "semantics/astRunner.h"

namespace Snapp::Native {

    NativeFunctionValue createPrintFunction() {
        return {
            DataType::Void,
            {DataType::Str},
            [](const std::vector<DataValue>& args) {
                std::cout << *coerceStr(args[0]) << std::endl;
                return std::nullopt;
            }
        };
    }

    NativeFunctionValue createInputFunction() {
        return {
            DataType::Str,
            {DataType::Str},
            [](const std::vector<DataValue>& args) {
                std::cout << *coerceStr(args[0]);
                std::string input;
                std::cin.ignore(); // unclear exactly what this fixes, but getline() doesn't work otherwise
                std::getline(std::cin, input);
                return input;
            }
        };
    }

}