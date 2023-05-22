#include <chrono>
#include "native/root/rootRandom.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "value/import.h"
#include "value/function.h"

namespace Snapp::Native {

    int randIntBase(int start, int end, int seed = std::chrono::system_clock::now().time_since_epoch().count()) {
        seed = (214013 * seed + 2531011);
        return start + (seed ^ seed >> 15) % (end - start + 1);
    }

    float randomBase(int seed = std::chrono::system_clock::now().time_since_epoch().count()) {
        seed = (214013 * seed + 2531011);
        return static_cast<float>((seed >> 16) & 0x7FFF) / 32767.0f;
    }

    ImportValue* createRootRandom() {
        auto scope = new Scope(nullptr, true);
        auto rootRandom = new ImportValue(scope);

        FunctionValue randInt;
        randInt.addOverload({ DataType::Int, { DataType::Int, DataType::Int, DataType::Int }, [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
            return randIntBase(std::get<int>(args[0]), std::get<int>(args[1]), std::get<int>(args[2]));
        } });
        randInt.addOverload({ DataType::Int, { DataType::Int, DataType::Int }, [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
            return randIntBase(std::get<int>(args[0]), std::get<int>(args[1]));
        } });

        FunctionValue random;
        random.addOverload({ DataType::Float, { DataType::Int }, [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
            return randomBase(std::get<int>(args[0]));
        } });
        random.addOverload({ DataType::Float, {}, [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
            return randomBase();
        } });

        scope->add("randInt", randInt);
        scope->add("random", random);

        return rootRandom;
    }
}
