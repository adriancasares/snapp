#include "native/root/rootArray.h"
#include "value/class.h"
#include "syntax/syntaxNode.h"

namespace Snapp::Native {

    void createArrayConstructor(FunctionValue& constructor) {
        constructor.addOverload({
            DataType::Void,
            {},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
                void* data = new std::vector<DataValue>();
                scope->assign("data_", data);

                return std::nullopt;
            }
        });
    }

    FunctionValue createArrayGet() {
        FunctionValue arrayGet;
        arrayGet.addOverload({
            DataType::Void,
            {DataType::Void},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
                void* data = std::get<void*>(scope->get("data_"));
                std::vector<DataValue>* dataVector = static_cast<std::vector<DataValue>*>(data);

                return dataVector->at(*coerceInt(args.at(0)));
            }
        });
        return arrayGet;
    }

    FunctionValue createArrayAdd() {
        FunctionValue arrayAdd;
        arrayAdd.addOverload({
            DataType::Void,
            {DataType::Void},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
                void* data = std::get<void*>(scope->get("data_"));
                std::vector<DataValue>* dataVector = static_cast<std::vector<DataValue>*>(data);
                dataVector->push_back(args.at(0));

                return std::nullopt;
            }
        });
        return arrayAdd;
    }

    ClassValue* createRootArray() {
        ClassValue* arrayClass = new ClassValue("Array");

        ClassAttribute dataAttribute = {
            {BaseDataType::Unknown, "data_", false},
            nullptr,
        };

        arrayClass->add("data_", dataAttribute);
        
        createArrayConstructor(arrayClass->constructor());
        
        arrayClass->add("get", createArrayGet());
        arrayClass->add("add", createArrayAdd());
        
        return arrayClass;
    }

}