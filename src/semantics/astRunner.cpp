#include "semantics/scope.h"
#include "semantics/astRunner.h"
#include "native/nativeGroup.h"
#include "error/runtimeError.h"
#include "value/object.h"

#include <cmath>

#define DEBUG_ONLY if (debugEnabled_)

namespace Snapp {

    void ASTRunner::runAST(const AbstractSyntaxTree& ast, bool enableDebug) {
        ASTRunner runner = ASTRunner(enableDebug);
        for (auto* node : ast.root()) {
            runner.runASTNode(node);
        }
    }

    ASTRunner::ASTRunner(bool enableDebug) {
        debugEnabled_ = enableDebug;
        scopes_.push_back(new Scope(nullptr, true));
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

    Scope& ASTRunner::currentStrongScope() {
        Scope* scope = scopes_[scopeIndex_];

        while (!scope->isStrong() && scope->hasParent()) {
          scope = scope->parent();
        }
        return *scope;
    }

    size_t ASTRunner::createScope(bool strong, bool isFunction, ClassValue* classValue) {
        // if a weak scope is being created, directly inherit all scopes of the parent (current)
        // if a strong scope is being created, use the uppermost scope as the parent
        if(strong) {
            Scope* parent = scopes_[scopeIndex_];

            while (parent->hasParent()) {
                parent = parent->parent();
            }

            size_t old = scopeIndex_;
            scopeIndex_ = scopes_.size();
            scopes_.push_back(new Scope(parent, strong, isFunction, classValue));
            return old;
        } else {
            size_t old = scopeIndex_;
            scopeIndex_ = scopes_.size();
            scopes_.push_back(new Scope(scopes_[old], strong, isFunction, classValue));
            return old;
        }
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
                    throw RuntimeError("unexpected type for unary operation '+'");
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
                    throw RuntimeError("unexpected type for unary operation '-'");
                }
            }

            if (unaryExpression->operation == Operation::BitwiseNot) {
                DataValue operand = *runASTNode(unaryExpression->operand);
                if (std::holds_alternative<IntValue>(operand)) {
                    return ~std::get<IntValue>(operand);
                }
                else {
                    throw RuntimeError("unexpected type for unary operation '~'");
                }
            }

            if (unaryExpression->operation == Operation::Not) {
                DataValue operand = *runASTNode(unaryExpression->operand);
                return !*convertBool(operand);
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
                    return *convertStr(left) + *convertStr(right);
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
                if (!*convertBool(left)) {
                    return false;
                }
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return *convertBool(right);
            }

            if (binaryExpression->operation == Operation::Or) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                if (*convertBool(left)) {
                    return true;
                }
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return *convertBool(right);
            }

            if (binaryExpression->operation == Operation::Assign) {
                auto* identifier = dynamic_cast<const SyntaxNodeIdentifier*>(binaryExpression->leftSide);

                DEBUG_ONLY std::cout << "Assigning " << identifier->name << std::endl;

                DataValue right = *runASTNode(binaryExpression->rightSide);

                currentScope().assign(identifier->name, right);

                return right;
            }

            if(binaryExpression->operation == Operation::Access) {
                auto* identifier = dynamic_cast<const SyntaxNodeIdentifier*>(binaryExpression->leftSide);
                auto* prop = dynamic_cast<const SyntaxNodeIdentifier*>(binaryExpression->rightSide);

                auto* objectValue = std::get_if<ObjectValue*>(&currentScope().get(identifier->name));

                if(!objectValue) {
                    throw RuntimeError("cannot access member of non-object value");
                }


                auto& member = (*objectValue)->scope()->get(prop->name);

                return member;
            }
         }

        if (auto* functionCall = dynamic_cast<const SyntaxNodeFunctionCall*>(node)) {
            DataValue callee = *runASTNode(functionCall->callee);

            if (auto* functionValue = std::get_if<FunctionValue>(&callee)) {
                std::vector<DataType> parameters;
                parameters.reserve(functionCall->arguments.size());
                for (auto* argument : functionCall->arguments) {
                    parameters.push_back(dataTypeOf(*runASTNode(argument)));
                }

                if (auto* overload = functionValue->getOverload(parameters)) {
                    return runFunction(*overload, functionCall);
                }
                throw RuntimeError("no matching function overload found");
            }
            else if (auto* classValue = std::get_if<ClassValue*>(&callee)) {
                std::vector<DataType> parameters;
                parameters.reserve(functionCall->arguments.size());

                for (auto* argument : functionCall->arguments) {
                    parameters.push_back(dataTypeOf(*runASTNode(argument)));
                }

                if (auto* overload = (*classValue)->constructor().getOverload(parameters)) {
                  ObjectValue *objectValue = new ObjectValue(*classValue);

                  for(auto& identifier : (*classValue)->identifiers()) {
                    if(auto* attribute = std::get_if<ClassAttribute>(&identifier.second)) {
                      SyntaxNode* initializer = attribute->initializer;
                      objectValue->scope()->add(identifier.first, *runASTNode(initializer));
                    } else if(auto* function = std::get_if<FunctionValue>(&identifier.second)) {
                      objectValue->scope()->add(identifier.first, *function);
                    } else {
                      throw RuntimeError("invalid class identifier");
                    }
                  }

                  runFunction(*overload, functionCall);

                  return objectValue;
                }
                throw RuntimeError("no matching class constructor found");
            } else {
                throw RuntimeError("attempt to call non-function");
            }
        }

        if (auto* variableDeclaration = dynamic_cast<const SyntaxNodeVariableDeclaration*>(node)) {
            if(currentStrongScope().isClass()) {
              ClassValue *classValue = currentStrongScope().getClass();

              ClassAttribute attribute = {
                  variableDeclaration->dataType,
                  variableDeclaration->value
              };

              DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Attribute (" << variableDeclaration->dataType << " " << variableDeclaration->identifier->name << std::endl;

              classValue->add(variableDeclaration->identifier->name, attribute);
            } else {
              DataValue value = *runASTNode(variableDeclaration->value);

              DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Variable (" << variableDeclaration->dataType << " " << variableDeclaration->identifier->name << "): " << *convertStr(value) << std::endl;

              currentScope().add(variableDeclaration->identifier->name, value);
            }
        }

        if (auto* blockStatement = dynamic_cast<const SyntaxNodeBlockStatement*>(node)) {
            size_t parent = createScope(false);
            std::optional<DataValue> value;

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] Block Statement: start" << std::endl;

            for (auto* statement : blockStatement->statements) {
                // FIXME: ideally signals like 'return' should propagate downwards to the nearest function
                if (auto* returnStatement = dynamic_cast<const SyntaxNodeReturnStatement*>(statement)) {
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
            size_t parent = createScope(false);
            std::optional<DataValue> value;
            if (*convertBool(*runASTNode(ifStatement->condition))) {
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
            size_t parent = createScope(false);
            while (*convertBool(*runASTNode(whileStatement->condition))) {
                DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] While Statement: iteration" << std::endl;

                runASTNode(whileStatement->loop);
            }

            DEBUG_ONLY std::cout << "[" << scopeIndex_ << "] While Statement: end" << std::endl;
            scopeIndex_ = parent;
            return {};
        }

        if (auto* forStatement = dynamic_cast<const SyntaxNodeForStatement*>(node)) {
            size_t parent = createScope(false);
            runASTNode(forStatement->initialize);

            while (*convertBool(*runASTNode(forStatement->condition))) {
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

            InterpretedFunction newFunction = {
                functionDeclaration->returnType,
                {},
                functionDeclaration->body,
            };

            for (auto parameter : functionDeclaration->parameters) {
                newFunction.parameters.push_back({parameter->dataType, parameter->identifier->name});
            }

            if(currentStrongScope().isClass()) {
              DEBUG_ONLY std::cout << "Method Declaration: " << functionDeclaration->output() << std::endl;
              ClassValue *classValue = currentStrongScope().getClass();
              if(classValue->has(name)) {
                DataValue& value = currentScope().get(name);
                if (auto* functionValue = std::get_if<FunctionValue>(&value)) {
                  functionValue->addOverload(newFunction);
                }
              } else {
                FunctionValue functionValue;
                functionValue.addOverload(newFunction);
                classValue->add(name, functionValue);
              }
            } else {
              DEBUG_ONLY std::cout << "Function Declaration: " << functionDeclaration->output() << std::endl;
              if (currentScope().has(name)) {
                DataValue& value = currentScope().get(name);
                if (auto* functionValue = std::get_if<FunctionValue>(&value)) {
                  functionValue->addOverload(newFunction);
                }
              }
              else {
                FunctionValue functionValue;
                functionValue.addOverload(newFunction);
                currentScope().add(functionDeclaration->identifier->name, functionValue);
              }
            }

            return {};
        }

        if(auto* constructorDeclaration = dynamic_cast<const SyntaxNodeConstructorDeclaration*>(node)) {
          if(currentStrongScope().isClass()) {
            ClassValue* classValue = currentStrongScope().getClass();

            DataType classType = DataType(BaseDataType::Object, classValue->name(), false);

            InterpretedFunction newFunction = {
                classType,
                {},
                constructorDeclaration->body,
                true
            };

            for (auto parameter : constructorDeclaration->parameters) {
                newFunction.parameters.push_back({parameter->dataType, parameter->identifier->name});
            }

            DEBUG_ONLY std::cout << "Constructor Declaration: " << constructorDeclaration->output() << std::endl;

            classValue->constructor().addOverload(newFunction);
          } else {
            throw std::runtime_error("Constructor declaration outside of class");
          }

          return {};
        }

        if (auto* classDeclaration = dynamic_cast<const SyntaxNodeClassDeclaration*>(node)) {
            ClassValue* classValue = new ClassValue(classDeclaration->identifier->name);

            currentScope().add(classDeclaration->identifier->name, classValue);

            size_t parent = createScope(true, false, classValue);

            runASTNode(classDeclaration->body);

            scopeIndex_ = parent;
            return classValue;
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

    std::optional<DataValue> ASTRunner::runFunction(const FunctionOverload& callee, const SyntaxNodeFunctionCall* functionCall) {
        if (auto* nativeFunctionValue = std::get_if<NativeFunction>(&callee)) {
            std::vector<DataValue> arguments;
            arguments.reserve(functionCall->arguments.size());
            for (auto* argument : functionCall->arguments) {
                arguments.push_back(*runASTNode(argument));
            }

            return nativeFunctionValue->body(arguments);
        }
        else if (auto* interpreted = std::get_if<InterpretedFunction>(&callee)) {
            size_t parent = createScope(true, true);
            for (size_t index = 0; index < interpreted->parameters.size(); index++) {
                currentScope().add(interpreted->parameters[index].name, *runASTNode(functionCall->arguments[index]));
            }
            auto returnValue = runASTNode(interpreted->body);
            scopeIndex_ = parent;
            return returnValue;
        }

        return {};
    }

}
