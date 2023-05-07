#include "value/function.h"
#include "value/object.h"

namespace Snapp {

    void FunctionValue::addOverload(const InterpretedFunction& overload) {
        overloads_.emplace_back(overload);
    }

    void FunctionValue::addOverload(const NativeFunction& overload) {
        overloads_.emplace_back(overload);
    }

    void FunctionValue::addOverload(const FunctionOverload& overload) {
        overloads_.emplace_back(overload);
    }

    std::vector<FunctionOverload>& FunctionValue::overloads() {
        return overloads_;
    }

    const std::vector<FunctionOverload>& FunctionValue::overloads() const {
        return overloads_;
    }

    const FunctionOverload* FunctionValue::getOverload(const std::vector<DataType>& parameters) const {
        for (auto& function: overloads_) {
            if (auto* interpreted = std::get_if<InterpretedFunction>(&function)) {
                if (interpreted->parameters.size() != parameters.size()) {
                    continue;
                }
                bool match = true;
                for (int i = 0; i < parameters.size(); i++) {
                    if (interpreted->parameters[i].type != parameters[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return &function;
                }
            }
            else if (auto* native = std::get_if<NativeFunction>(&function)) {
                if (native->parameters.size() != parameters.size()) {
                    continue;
                }
                bool match = true;
                for (int i = 0; i < parameters.size(); i++) {
                    if (native->parameters[i].base() != parameters[i].base()) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return &function;
                }
            }
        }
        return nullptr;
    }

    void FunctionValue::bind(ObjectValue* object) {
        scope_ = object->scope();
    }

    Scope* FunctionValue::scope() const {
        return *scope_;
    }

    void FunctionValue::setScope(Scope* scope) {
        scope_ = scope;
    }
};