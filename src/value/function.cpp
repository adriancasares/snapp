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
        for (auto& overload: overloads_) {
            if (anyParameters_) {
                return &overload;
            }

            if (auto* interpreted = std::get_if<InterpretedFunction>(&overload)) {
                if (parameters.size() != interpreted->parameters.size()) {
                    continue;
                }
                bool match = true;
                for (size_t i = 0; i < parameters.size(); i++) {
                    if (interpreted->parameters[i].type != parameters[i] && interpreted->parameters[i].type != DataType::Void) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return &overload;
                }
            }
            else if (auto* native = std::get_if<NativeFunction>(&overload)) {
                if (parameters.size() != native->parameters.size()) {
                    continue;
                }
                bool match = true;
                for (size_t i = 0; i < parameters.size(); i++) {
                    if (native->parameters[i] != parameters[i] && native->parameters[i] != DataType::Void) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return &overload;
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