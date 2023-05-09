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
            void* data = new std::vector<DataValue>();

            scope->assign("data_", data);

            return std::nullopt;
          }
      };
    };

    FunctionValue createAdd () {
      FunctionValue addToArray;
      addToArray.addOverload({
                                    DataType::Void,
                                    {DataType::Void},
                                    [](const std::vector<DataValue> &args,
                                        Snapp::Scope *scope) -> std::optional<DataValue> {

                                      void *data = std::get<void *>(scope->get("data_"));

                                      std::vector<DataValue> *dataVector = static_cast<std::vector<DataValue> *>(data);
                                      dataVector->push_back(args[0]);

                                      return std::nullopt;
                                    }
                                });
        return addToArray;
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

        arrayClass->add("add", createAdd());
        return arrayClass;
    }
}