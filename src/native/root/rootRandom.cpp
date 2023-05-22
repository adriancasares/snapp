#include <chrono>
#include "native/root/rootRandom.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "value/import.h"
namespace Snapp::Native {

    int randInt(int start, int end, int seed = std::chrono::system_clock::now().time_since_epoch().count()) {
        seed = (214013 * seed + 2531011);
        return start + (seed ^ seed >> 15) % (end - start + 1);
    }

    float random(int seed = std::chrono::system_clock::now().time_since_epoch().count()) {
        seed = (214013 * seed + 2531011);
        return static_cast<float>((seed >> 16) & 0x7FFF) / 32767.0f;
    }

    ImportValue* createRootRandom() {
        auto scope = new Scope(nullptr, true);
        auto rootRandom = new ImportValue(scope);

        scope->add("randInt", new FunctionValue(DataType::Int, { {DataType::Int, "start"}, {DataType::Int, "end"}, {DataType::Int, "seed"} }, randInt));
        scope->add("random", new FunctionValue(DataType::Float, { {DataType::Int, "seed"} }, random));

        return rootRandom;
    }
}
