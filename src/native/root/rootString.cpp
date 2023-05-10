#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "value/import.h"

namespace Snapp::Native {

    FunctionValue createLengthFunction() {
        FunctionValue lengthFunction;
        lengthFunction.addOverload({
            DataType::Int,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
                std::string str = *coerceStr(args.at(0));
                int length = str.length();
                return length;
            }
        });
        return lengthFunction;
    }

    ImportValue* createRootString() {
        auto* scope = new Scope(nullptr, true);
        auto* rootString = new ImportValue(scope);

        scope->add("length", createLengthFunction());
        
        return rootString;
    }

}