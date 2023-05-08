//
// Created by Adrian Casares on 5/8/23.
//
#include "native/root/rootArray.h"
#include "value/class.h"
#include "syntax/syntaxNode.h"
namespace Snapp::Native {

    NativeFunction createArrayConstructor() {
      return {
          DataType::Void,
          {},
          [](const std::vector<DataValue>& args, Snapp::Scope* scope) -> std::optional<DataValue> {
            void* data = malloc(1000);

            scope->assign("data_", data);

            return std::nullopt;
          }
      };
    };

    FunctionValue createGetAddress () {
      FunctionValue getAddress;

      getAddress.addOverload({
        DataType::Void,
        {},
        [](const std::vector<DataValue>& args, Snapp::Scope* scope) -> std::optional<DataValue> {
          void* data = std::get<void*>(scope->get("data_"));

          std::cout << data << std::endl;

            return std::nullopt;
        }
      });

        return getAddress;
    }

    ClassValue* createRootArray() {
        ClassValue* arrayClass = new ClassValue("Array");

        SyntaxNodeLiteral* data = new SyntaxNodeLiteral(0);

        DataType dataType = {BaseDataType::Unknown, "data_", false};

        ClassAttribute dataAttribute = {
            dataType,
            data
        };

        arrayClass->add("data_", dataAttribute);

        arrayClass->constructor().addOverload(createArrayConstructor());

        arrayClass->add("getAddress", createGetAddress());
        return arrayClass;
    }
}