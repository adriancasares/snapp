//
// Created by Adrian Casares on 5/2/23.
//
#include "syntax/functionGroup.h"

namespace Snapp {

    FunctionGroup::FunctionGroup() {
    }

    void FunctionGroup::addFunction(const FunctionValue& function) {
        functions_.push_back(function);
    }

    void FunctionGroup::addFunction(const NativeFunctionValue& function) {
        functions_.push_back(function);
    }

    std::vector<std::variant<FunctionValue, NativeFunctionValue>>& FunctionGroup::functions() {
        return functions_;
    }

    const std::vector<std::variant<FunctionValue, NativeFunctionValue>>& FunctionGroup::functions() const {
        return functions_;
    }

    bool FunctionGroup::hasFunction(const std::vector<DataType>& parameters) const {
      for (const auto &function : functions_) {
        if (std::holds_alternative<FunctionValue>(function)) {
          const auto &functionValue = std::get<FunctionValue>(function);
          if (functionValue.parameters.size() == parameters.size()) {
            bool match = true;
            for (int i = 0; i < parameters.size(); i++) {
              if (functionValue.parameters[i].type.base() != parameters[i].base()) {
                match = false;
                break;
              }
            }
            if (match) {
              return true;
            }
          }
        } else if(std::holds_alternative<NativeFunctionValue>(function)) {
          const auto &functionValue = std::get<NativeFunctionValue>(function);
          if (functionValue.parameters.size() == parameters.size()) {
            bool match = true;
            for (int i = 0; i < parameters.size(); i++) {
              if (functionValue.parameters[i].base() != parameters[i].base()) {
                match = false;
                break;
              }
            }
            if (match) {
              return true;
            }
          }
        }
      }
    }

    std::variant<FunctionValue, NativeFunctionValue>& FunctionGroup::getFunction(const std::vector<DataType>& parameters) {
      for (auto &function : functions_) {
        if (std::holds_alternative<FunctionValue>(function)) {
          const auto &functionValue = std::get<FunctionValue>(function);
          if (functionValue.parameters.size() == parameters.size()) {
            bool match = true;
            for (int i = 0; i < parameters.size(); i++) {
              if (functionValue.parameters[i].type.base() != parameters[i].base()) {
                match = false;
                break;
              }
            }
            if (match) {
              return function;
            }
          }
        } else if (std::holds_alternative<NativeFunctionValue>(function)) {
          const auto &functionValue = std::get<NativeFunctionValue>(function);
          if (functionValue.parameters.size() == parameters.size()) {
            bool match = true;
            for (int i = 0; i < parameters.size(); i++) {
              if (functionValue.parameters[i].base() != parameters[i].base()) {
                match = false;
                break;
              }
            }
            if (match) {
              return function;
            }
          }
        }
      }
    }
};