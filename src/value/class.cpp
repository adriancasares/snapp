#include "value/class.h"
# include "value/dataValue.h"

namespace Snapp {

    Scope* ClassValue::scope() const {
        return scope_;
    }

    const std::vector<FunctionValue>& ClassValue::constructors() const {
        return constructors_;
    }

    void ClassValue::addConstructor(const FunctionValue& constructor) {
        constructors_.push_back(constructor);
    }

    void ClassValue::setScope(Scope* scope) {
        scope_ = scope;
    }

    bool ClassValue::hasConstructor(const std::vector<DataType> &arguments) const {
        for (const auto& constructor : constructors_) {
            bool matches = true;
            for (size_t i = 0; i < constructor.parameters.size() && matches; i++) {
                const DataType parameterType = arguments[i];
                if (constructor.parameters[i].type != parameterType) {
                    matches = false;
                }
            }

            if (matches) {
                return true;
            }
        }
        return false;
    }

    FunctionValue* ClassValue::getConstructor(const std::vector<DataType> &arguments) {
        for (auto& constructor : constructors_) {
            bool matches = true;
            for (size_t i = 0; i < constructor.parameters.size() && matches; i++) {
                const DataType parameterType = arguments[i];
                if (constructor.parameters[i].type != parameterType) {
                    matches = false;
                }
            }

            if (matches) {
                return &constructor;
            }
        }
        return nullptr;
    }

}