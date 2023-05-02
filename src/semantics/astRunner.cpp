//
// Created by Adrian Casares on 4/17/23.
//

#include <cmath>
#include "semantics/astRunner.h"
#include "native/nativeGroup.h"
#include "syntax/dataType.h"

#define DEBUG_ONLY if (debugEnabled_)

namespace Snapp {

    std::optional<IntValue> coerceInt(DataValue value) {
      if (auto* x = std::get_if<IntValue>(&value)) {
        return *x;
      }
      if (auto* x = std::get_if<IntValue>(&value)) {
        return *x;
      }
      else {
        return {};
      }
    }

    std::optional<FloatValue> coerceFloat(DataValue value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        if (auto* x = std::get_if<FloatValue>(&value)) {
            return *x;
        }
        else {
            return {};
        }
    }

    std::optional<BoolValue> coerceBool(DataValue value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return *x;
        }
        else if (auto* x = std::get_if<FloatValue>(&value)) {
            return *x;
        }
        else if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x;
        }
        else if (auto* x = std::get_if<StrValue>(&value)) {
            return !x->empty();
        }
        else {
            return {};
        }
    }

    std::optional<StrValue> coerceStr(DataValue value) {
        if (auto* x = std::get_if<IntValue>(&value)) {
            return std::to_string(*x);
        }
        else if (auto* x = std::get_if<FloatValue>(&value)) {
            return std::to_string(*x);
        }
        else if (auto* x = std::get_if<BoolValue>(&value)) {
            return *x ? "true" : "false";
        }
        else if (auto* x = std::get_if<StrValue>(&value)) {
            return *x;
        }
        else {
            return {};
        }
    }

    void ASTRunner::runAST(const AbstractSyntaxTree& ast, bool enableDebug) {
        ASTRunner runner = ASTRunner(enableDebug);
        for (auto* node : ast.root()) {
            runner.runASTNode(node);
        }
    }

    std::optional<DataValue> ASTRunner::runFunction(const std::variant<FunctionValue, NativeFunctionValue>& callee, const SyntaxNodeFunctionCall* functionCall) {
      if(std::holds_alternative<NativeFunctionValue>(callee)) {
        NativeFunctionValue nativeFunction = std::get<NativeFunctionValue>(callee);

        std::vector<DataValue> arguments;

        for (auto *argument : functionCall->arguments) {
          arguments.push_back(*runASTNode(argument));
        }

        return nativeFunction.body(arguments);
      } else if(std::holds_alternative<FunctionValue>(callee)) {
        FunctionValue function = std::get<FunctionValue>(callee);

        int parent = createScope(true);

        for (int parameterIndex = 0; parameterIndex < function.parameters.size(); parameterIndex++) {
          currentScope().add(function.parameters[parameterIndex].name,
                             *runASTNode(functionCall->arguments[parameterIndex]));
        }

        return runASTNode(function.body);
      }
    }

    ASTRunner::ASTRunner(bool enableDebug) {
        debugEnabled_ = enableDebug;
        scopes_.push_back(new Scope());
        scopeIndex_ = 0;
        Native::injectNativeGroup(scopes_[scopeIndex_]);
    }

    ASTRunner::~ASTRunner() {
        for (Scope* scope : scopes_) {
            delete scope;
        }
    }

    Scope& ASTRunner::currentScope() {
        return *scopes_[scopeIndex_];
    }

    size_t ASTRunner::createScope(bool isFunction) {
        size_t parent = scopeIndex_;
        scopeIndex_ = scopes_.size();
        scopes_.push_back(new Scope(scopes_[parent], isFunction));
        return parent;
    }

    std::optional<DataValue> ASTRunner::runASTNode(const SyntaxNode *node) {
        if (node == nullptr) {
            return {};
        }

        if (auto* literal = dynamic_cast<const SyntaxNodeLiteral*>(node)) {
            return literal->value;
        }

        if (auto* identifier = dynamic_cast<const SyntaxNodeIdentifier*>(node)) {
            return currentScope().get(identifier->name);
        }

        if (auto* unaryExpression = dynamic_cast<const SyntaxNodeUnaryExpression*>(node)) {
            if (unaryExpression->operation == Operation::ToNumber) {
                DataValue operand = *runASTNode(unaryExpression->operand);
                if (std::holds_alternative<IntValue>(operand)) {
                    return +std::get<IntValue>(operand);
                }
                else if (std::holds_alternative<FloatValue>(operand)) {
                    return +std::get<FloatValue>(operand);
                }
                else {
                    return {}; // TODO error
                }
            }

            if (unaryExpression->operation == Operation::Negate) {
                DataValue operand = *runASTNode(unaryExpression->operand);
                if (std::holds_alternative<IntValue>(operand)) {
                    return -std::get<IntValue>(operand);
                }
                else if (std::holds_alternative<FloatValue>(operand)) {
                    return -std::get<FloatValue>(operand);
                }
                else {
                    return {}; // TODO error
                }
            }

            if (unaryExpression->operation == Operation::BitwiseNot) {
                DataValue operand = *runASTNode(unaryExpression->operand);
                if (std::holds_alternative<IntValue>(operand)) {
                    return ~std::get<IntValue>(operand);
                }
                else {
                    return {}; // TODO error
                }
            }

            if (unaryExpression->operation == Operation::Not) {
                DataValue operand = *runASTNode(unaryExpression->operand);
                return !*coerceBool(operand);
            }
        }

        if (auto* binaryExpression = dynamic_cast<const SyntaxNodeBinaryExpression*>(node)) {
            if (binaryExpression->operation == Operation::Exponent) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::pow(*coerceFloat(left), *coerceFloat(right));
            }

            if (binaryExpression->operation == Operation::Multiply) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) * std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) * *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Divide) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) / std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) / *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Modulus) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) % std::get<IntValue>(right);
                }
                else {
                    return std::fmod(*coerceFloat(left), *coerceFloat(right));
                }
            }

            if (binaryExpression->operation == Operation::Add) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) || std::holds_alternative<StrValue>(right)) {
                    return *coerceStr(left) + *coerceStr(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) + std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) + *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Subtract) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) - std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) - *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::LessThan) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) && std::holds_alternative<StrValue>(right)) {
                    return std::get<StrValue>(left) == std::get<StrValue>(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) < std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) < *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::GreaterThan) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) && std::holds_alternative<StrValue>(right)) {
                    return std::get<StrValue>(left) == std::get<StrValue>(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) > std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) > *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::LessEqual) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) && std::holds_alternative<StrValue>(right)) {
                    return std::get<StrValue>(left) == std::get<StrValue>(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) <= std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) <= *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::GreaterEqual) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) && std::holds_alternative<StrValue>(right)) {
                    return std::get<StrValue>(left) == std::get<StrValue>(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) >= std::get<IntValue>(right);
                }
                else {
                    return *coerceFloat(left) >= *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Equal) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) && std::holds_alternative<StrValue>(right)) {
                    return std::get<StrValue>(left) == std::get<StrValue>(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) == std::get<IntValue>(right);
                }
                else if (std::holds_alternative<BoolValue>(left) && std::holds_alternative<BoolValue>(right)) {
                    return std::get<BoolValue>(left) == std::get<BoolValue>(right);
                }
                else {
                    return *coerceFloat(left) == *coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::NotEqual) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<StrValue>(left) && std::holds_alternative<StrValue>(right)) {
                    return std::get<StrValue>(left) != std::get<StrValue>(right);
                }
                else if (std::holds_alternative<IntValue>(left) && std::holds_alternative<IntValue>(right)) {
                    return std::get<IntValue>(left) != std::get<IntValue>(right);
                }
                else if (std::holds_alternative<BoolValue>(left) && std::holds_alternative<BoolValue>(right)) {
                    return std::get<BoolValue>(left) != std::get<BoolValue>(right);
                }
                else {
                    return *coerceFloat(left) != *coerceFloat(right);
                }
            }

            // Bitwise operations
            if (binaryExpression->operation == Operation::BitwiseAnd) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::get<IntValue>(left) & std::get<IntValue>(right);
            }

            if (binaryExpression->operation == Operation::BitwiseXor) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::get<IntValue>(left) ^ std::get<IntValue>(right);
            }

            if (binaryExpression->operation == Operation::BitwiseOr) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::get<IntValue>(left) | std::get<IntValue>(right);
            }

            // Logical operations
            if (binaryExpression->operation == Operation::And) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                if (!*coerceBool(left)) {
                    return false;
                }
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return *coerceBool(right);
            }

            if (binaryExpression->operation == Operation::Or) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                if (*coerceBool(left)) {
                    return true;
                }
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return *coerceBool(right);
            }

            if (binaryExpression->operation == Operation::Assign) {
                auto* identifier = dynamic_cast<const SyntaxNodeIdentifier*>(binaryExpression->leftSide);

                DEBUG_ONLY std::cout << "Assigning " << identifier->name << std::endl;

                DataValue right = *runASTNode(binaryExpression->rightSide);

                currentScope().assign(identifier->name, right);

                return right;
            }
        }

        if (auto* functionCall = dynamic_cast<const SyntaxNodeFunctionCall*>(node)) {
            DataValue callee = *runASTNode(functionCall->callee);

          if (std::holds_alternative<FunctionValue>(callee)) {
            runFunction(std::get<FunctionValue>(callee), functionCall);
          }
          else if (std::holds_alternative<NativeFunctionValue>(callee)) {
              runFunction(std::get<NativeFunctionValue>(callee), functionCall);
          }
          else if(std::holds_alternative<FunctionGroup>(callee)) {
            FunctionGroup group = std::get<FunctionGroup>(callee);

            std::vector<DataType> parameters;

              for(auto* argument : functionCall->arguments) {
                  parameters.push_back(DataType::getDataType(*runASTNode(argument)));
              }

              if(group.hasFunction(parameters)) {
                runFunction(group.getFunction(parameters), functionCall);
              }

              else {
                throw std::runtime_error("No matching function found");
              }
          }
        }

        if (auto* variableDeclaration = dynamic_cast<const SyntaxNodeVariableDeclaration*>(node)) {
            DataValue value = *runASTNode(variableDeclaration->value);

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Variable (" << variableDeclaration->dataType << " " << variableDeclaration->identifier->name << "): " << *coerceStr(value) << std::endl;

            currentScope().add(variableDeclaration->identifier->name, value);
        }

        if (auto* blockStatement = dynamic_cast<const SyntaxNodeBlockStatement*>(node)) {
            size_t parent = createScope();
            std::optional<DataValue> value;

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Block Statement: start" << std::endl;

            for (auto* statement : blockStatement->statements) {
                // FIXME: ideally signals like 'return' should propagate downwards to the nearest function
                if (auto* returnStatement = dynamic_cast<const SyntaxNodeReturnStatement *>(statement)) {
                    DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Block Statement: return" << std::endl;

                    value = runASTNode(returnStatement->value);
                    return value;
                } else {
                    value = runASTNode(statement);
                }
            }

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Block Statement: end" << std::endl;

            scopeIndex_ = parent;
            return value;
        }

        if (auto* ifStatement = dynamic_cast<const SyntaxNodeIfStatement*>(node)) {
            size_t parent = createScope();
            std::optional<DataValue> value;
            if (*coerceBool(*runASTNode(ifStatement->condition))) {
                DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] If Statement: condition is true" << std::endl;

                value = runASTNode(ifStatement->consequent);
            } else {
                DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] If Statement: condition is false" << std::endl;

                value = runASTNode(ifStatement->alternative);
            }

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] If Statement: end" << std::endl;

            scopeIndex_ = parent;
            return value;
        }

        if (auto* whileStatement = dynamic_cast<const SyntaxNodeWhileStatement*>(node)) {
            size_t parent = createScope();
            while (*coerceBool(*runASTNode(whileStatement->condition))) {
                DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] While Statement: iteration" << std::endl;

                runASTNode(whileStatement->loop);
            }

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] While Statement: end" << std::endl;
            scopeIndex_ = parent;
            return {};
        }

        if (auto* forStatement = dynamic_cast<const SyntaxNodeForStatement*>(node)) {
            size_t parent = createScope();
            runASTNode(forStatement->initialize);

            while (*coerceBool(*runASTNode(forStatement->condition))) {
                DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] For Statement: iteration" << std::endl;

                runASTNode(forStatement->loop);
                runASTNode(forStatement->update);
            }

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] For Statement: end" << std::endl;

            scopeIndex_ = parent;
            return {};
        }

        if (auto* returnStatement = dynamic_cast<const SyntaxNodeReturnStatement*>(node)) {
            DEBUG_ONLY std::cout << "Return Statement: " << returnStatement->output() << std::endl;

            runASTNode(returnStatement->value);
        }

        if (auto* functionDeclaration = dynamic_cast<const SyntaxNodeFunctionDeclaration*>(node)) {
          std::string name = functionDeclaration->identifier->name;

          DEBUG_ONLY std::cout << "Function Declaration: " << functionDeclaration->output() << std::endl;

          FunctionValue functionValue = {
              functionDeclaration->returnType,
              {},
              functionDeclaration->body
          };

          for (auto parameter : functionDeclaration->parameters) {
              FunctionValue::Parameter parameterValue = {parameter->dataType, parameter->identifier->name};
              functionValue.parameters.push_back(parameterValue);
          };

          if(currentScope().exists(name)) {
            DataValue& value = currentScope().get(name);

            for(auto parameter : functionDeclaration->parameters) {
              FunctionValue::Parameter parameterValue = {parameter->dataType, parameter->identifier->name};
              functionValue.parameters.push_back(parameterValue);
            };

            if(std::holds_alternative<FunctionValue>(value)) {
              FunctionGroup functionGroupValue = FunctionGroup();
              functionGroupValue.addFunction(std::get<FunctionValue>(value));
              functionGroupValue.addFunction(functionValue);

              currentScope().assign(name, functionGroupValue);
            }
            else if(std::holds_alternative<FunctionGroup>(value)) {
              FunctionGroup functionGroupValue = std::get<FunctionGroup>(value);
              functionGroupValue.addFunction(functionValue);
              currentScope().assign(name, functionValue);
            }
          } else {
            currentScope().add(functionDeclaration->identifier->name, functionValue);
          }
        }

        if (auto* classDeclaration = dynamic_cast<const SyntaxNodeClassDeclaration*>(node)) {
//          addIdentifier(classDeclaration->identifier->name, classDeclaration);
        }

        if (auto* observerDeclaration = dynamic_cast<const SyntaxNodeObserverDeclaration*>(node)) {
//          DEBUG_ONLY std::cout << "Observer Declaration: " << observerDeclaration->output() << std::endl;
//          runASTNode(observerDeclaration->identifier);
//          for (auto &argument : observerDeclaration->arguments) {
//              runASTNode(argument);
//          }
        }
        
        return {};
    }

}
