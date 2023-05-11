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
        int insertedParams = boundStr_ ? 1 : 0;

        for (auto& function: overloads_) {
            if (anyParameters_) {
                return &function;
            }

            if (auto* interpreted = std::get_if<InterpretedFunction>(&function)) {
                if (parameters.size() != interpreted->parameters.size() - insertedParams) {
                    continue;
                }
                bool match = true;
                for (int i = 0; i < parameters.size(); i++) {
                    if (interpreted->parameters[insertedParams + i].type != parameters[i] && interpreted->parameters[insertedParams + i].type != DataType::Void) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return &function;
                }
            }
            else if (auto* native = std::get_if<NativeFunction>(&function)) {
                if (parameters.size() != native->parameters.size() - insertedParams) {
                    continue;
                }
                bool match = true;
                for (int i = 0; i < parameters.size(); i++) {
                    if (native->parameters[insertedParams + i].base() != parameters[i].base() && native->parameters[insertedParams + i].base() != BaseDataType::Void) {
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

    Scope* FunctionValue::scope() const {
        return *scope_;
    }

    void FunctionValue::setScope(Scope* scope) {
        scope_ = scope;
    }

    void FunctionValue::bind(ObjectValue* objectValue) {
        scope_ = objectValue->scope();
    }

    const std::optional<StrValue>& FunctionValue::boundStr() const {
        return boundStr_;
    }

    void FunctionValue::bind(const StrValue& strValue) {
        boundStr_ = strValue;
    }

    void FunctionValue::setAnyParameters(bool anyParameters) {
        anyParameters_ = anyParameters;
    }

    bool FunctionValue::anyParameters() const {
        return anyParameters_;
    }

}