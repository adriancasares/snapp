//
// Created by Adrian Casares on 4/17/23.
//
// function to run the AST

#include <cmath>
#include "semantics/astRunner.h"

namespace Snapp {

    namespace {

        double coerceFloat(DataValue value) {
            if (int* i = std::get_if<int>(&value)) {
                return *i;
            }
            else {
                return std::get<double>(value);
            }
        }

        bool coerceBool(DataValue value) {
            if (int* i = std::get_if<int>(&value)) {
                return *i;
            }
            else if (double* d = std::get_if<double>(&value)) {
                return *d;
            }
            else if (std::string* s = std::get_if<std::string>(&value)) {
                return !s->empty();
            }
            else {
                return std::get<bool>(value);
            }
        }

        std::string coerceStr(DataValue value) {
            if (int* i = std::get_if<int>(&value)) {
                return std::to_string(*i);
            }
            else if (double* d = std::get_if<double>(&value)) {
                return std::to_string(*d);
            }
            else if (bool* b = std::get_if<bool>(&value)) {
                return *b ? "true" : "false";
            }
            else {
                return std::get<std::string>(value);
            }
        }

    }

    std::map<std::string, DataValue> ASTRunner::identifiers;

    void ASTRunner::addIdentifier(std::string identifier, DataValue value) {
        // check if identifier already exists
        if (identifiers.find(identifier) != identifiers.end()) {
            std::cout << "Identifier already exists: " << identifier << std::endl;
        } else {
            identifiers[identifier] = value;
            std::cout << "Added identifier: " << identifier << std::endl;
        }
    }

    std::optional<DataValue> ASTRunner::runASTNode(const SyntaxNode *node) {
        if (node == nullptr) {
            return {};
        }

        else if (auto* literal = dynamic_cast<const SyntaxNodeLiteral*>(node)) {
            return literal->value;
        }

        else if (auto* identifier = dynamic_cast<const SyntaxNodeIdentifier*>(node)) {
//          std::cout << "Identifier: " << identifier->output() << std::endl;
        }

        else if (auto* unaryExpression = dynamic_cast<const SyntaxNodeUnaryExpression*>(node)) {
//          std::cout << "Unary Expression: " << unaryExpression->output() << std::endl;
//          runASTNode(unaryExpression->operand);
        }

        else if (auto* binaryExpression = dynamic_cast<const SyntaxNodeBinaryExpression*>(node)) {
            if (binaryExpression->operation == Operation::Exponent) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::pow(coerceFloat(left), coerceFloat(right));
            }

            if (binaryExpression->operation == Operation::Multiply) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) * std::get<int>(right);
                }
                else {
                    return coerceFloat(left) * coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Divide) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) / std::get<int>(right);
                }
                else {
                    return coerceFloat(left) / coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Modulus) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) % std::get<int>(right);
                }
                else {
                    return std::fmod(coerceFloat(left), coerceFloat(right));
                }
            }

            if (binaryExpression->operation == Operation::Add) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) || std::holds_alternative<std::string>(right)) {
                    std::cout << "String concatenation" << std::endl;
                    return coerceStr(left) + coerceStr(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    std::cout << "Integer addition" << std::endl;
                    return std::get<int>(left) + std::get<int>(right);
                }
                else {
                    std::cout << "Float addition" << std::endl;
                    return coerceFloat(left) + coerceFloat(right);
                }

                /*if (std::holds_alternative<int>(*left) && std::holds_alternative<int>(*right)) {
                    std::cout << "Adding two ints" << std::endl;
                    int* result = new int;
                    *result = std::get<int>(*left) + std::get<int>(*right);
                    return result;
                }

                if (std::holds_alternative<float>(*left) && std::holds_alternative<float>(*right)) {
                    std::cout << "Adding two floats" << std::endl;
                    float* result = new float;
                    *result = std::get<float>(*left) + std::get<float>(*right);
                    return result;
                }

                if (std::holds_alternative<float>(*left) && std::holds_alternative<int>(*right)) {
                    std::cout << "Adding a float and an int" << std::endl;
                    float* result = new float;
                    *result = std::get<float>(*left) + std::get<int>(*right);
                    return result;
                }

                if (std::holds_alternative<int>(*left) && std::holds_alternative<float>(*right)) {
                    std::cout << "Adding an int and a float" << std::endl;
                    float* result = new float;
                    *result = std::get<int>(*left) + std::get<float>(*right);
                    return result;
                }

                if (std::holds_alternative<std::string>(*left) && std::holds_alternative<std::string>(*right)) {
                    std::cout << "Adding two strings" << std::endl;
                    std::string* result = new std::string;
                    *result = std::get<std::string>(*left) + std::get<std::string>(*right);
                    return result;
                }

                std::cout << "Error: Cannot add these types" << std::endl;*/
            }

            if (binaryExpression->operation == Operation::Subtract) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) - std::get<int>(right);
                }
                else {
                    return coerceFloat(left) - coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::LessThan) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) == std::get<std::string>(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) < std::get<int>(right);
                }
                else {
                    return coerceFloat(left) < coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::GreaterThan) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) == std::get<std::string>(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) > std::get<int>(right);
                }
                else {
                    return coerceFloat(left) > coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::LessEqual) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) == std::get<std::string>(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) <= std::get<int>(right);
                }
                else {
                    return coerceFloat(left) <= coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::GreaterEqual) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) == std::get<std::string>(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) >= std::get<int>(right);
                }
                else {
                    return coerceFloat(left) >= coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::Equal) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) == std::get<std::string>(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) == std::get<int>(right);
                }
                else if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) {
                    return std::get<bool>(left) == std::get<bool>(right);
                }
                else {
                    return coerceFloat(left) == coerceFloat(right);
                }
            }

            if (binaryExpression->operation == Operation::NotEqual) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                    return std::get<std::string>(left) != std::get<std::string>(right);
                }
                else if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
                    return std::get<int>(left) != std::get<int>(right);
                }
                else if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) {
                    return std::get<bool>(left) != std::get<bool>(right);
                }
                else {
                    return coerceFloat(left) != coerceFloat(right);
                }
            }

            // Bitwise operations
            if (binaryExpression->operation == Operation::BitwiseAnd) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::get<int>(left) & std::get<int>(right);
            }

            if (binaryExpression->operation == Operation::BitwiseXor) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::get<int>(left) ^ std::get<int>(right);
            }

            if (binaryExpression->operation == Operation::BitwiseOr) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return std::get<int>(left) | std::get<int>(right);
            }

            // Logical operations
            if (binaryExpression->operation == Operation::And) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                if (!coerceBool(left)) {
                    return false;
                }
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return coerceBool(right);
            }

            if (binaryExpression->operation == Operation::Or) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                if (coerceBool(left)) {
                    return true;
                }
                DataValue right = *runASTNode(binaryExpression->rightSide);
                return coerceBool(right);
            }

            if (binaryExpression->operation == Operation::Assign) {
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                // TODO
                return right;
            }
        }

        else if (auto* functionCall = dynamic_cast<const SyntaxNodeFunctionCall*>(node)) {
//          std::cout << "Function Call: " << functionCall->output() << std::endl;
//          runASTNode(functionCall->callee);
//          for (auto &argument : functionCall->arguments) {
//                runASTNode(argument);
//          }
        }

        else if (auto* variableDeclaration = dynamic_cast<const SyntaxNodeVariableDeclaration*>(node)) {
            std::optional<DataValue> valueOpt = runASTNode(variableDeclaration->value);

            if (variableDeclaration->dataType.base() == BaseDataType::Int) {
                int value = valueOpt ? std::get<int>(*valueOpt) : 0;
                std::cout << "Value: " << value << std::endl;
                addIdentifier(variableDeclaration->identifier->name, value);
            }

            if (variableDeclaration->dataType.base() == BaseDataType::Float) {
                double value = valueOpt ? coerceFloat(*valueOpt) : 0;
                std::cout << "Value: " << value << std::endl;
                addIdentifier(variableDeclaration->identifier->name, value);
            }

            if (variableDeclaration->dataType.base() == BaseDataType::Bool) {
                bool value = valueOpt ? coerceBool(*valueOpt) : false;
                std::cout << "Value: " << value << std::endl;
                addIdentifier(variableDeclaration->identifier->name, value);
            }

            if (variableDeclaration->dataType.base() == BaseDataType::Str) {
                std::string value = valueOpt ? std::get<std::string>(*valueOpt) : "";
                std::cout << "Value: " << value << std::endl;
                addIdentifier(variableDeclaration->identifier->name, value);
            }


            if (variableDeclaration->dataType.base() == BaseDataType::Object) {
                std::cout << "Cannot declare object variable" << std::endl;
            }

            if (variableDeclaration->dataType.base() == BaseDataType::Void) {
                std::cout << "Cannot declare void variable" << std::endl;
            }

            if (variableDeclaration->dataType.base() == BaseDataType::Unknown) {
                std::cout << "Cannot declare unknown variable" << std::endl;
            }
        }

        else if (auto* blockStatement = dynamic_cast<const SyntaxNodeBlockStatement*>(node)) {
//          std::cout << "Block Statement: " << blockStatement->output() << std::endl;
//          for (auto &statement : blockStatement->statements) {
//              runASTNode(statement);
//          }
        }

        else if (auto* ifStatement = dynamic_cast<const SyntaxNodeIfStatement*>(node)) {
//          std::cout << "If Statement: " << ifStatement->output() << std::endl;
//          runASTNode(ifStatement->condition);
//          runASTNode(ifStatement->consequent);
//          runASTNode(ifStatement->alternative);
        }

        else if (auto* whileStatement = dynamic_cast<const SyntaxNodeWhileStatement*>(node)) {
//          std::cout << "While Statement: " << whileStatement->output() << std::endl;
//          runASTNode(whileStatement->condition);
//          runASTNode(whileStatement->body);
        }

        else if (auto* forStatement = dynamic_cast<const SyntaxNodeForStatement*>(node)) {
//          std::cout << "For Statement: " << forStatement->output() << std::endl;
//          runASTNode(forStatement->initialization);
//          runASTNode(forStatement->condition);
//          runASTNode(forStatement->increment);
//          runASTNode(forStatement->body);
        }

        else if (auto* returnStatement = dynamic_cast<const SyntaxNodeReturnStatement*>(node)) {
//          std::cout << "Return Statement: " << returnStatement->output() << std::endl;
//          runASTNode(returnStatement->value);
        }

        else if (auto* functionDeclaration = dynamic_cast<const SyntaxNodeFunctionDeclaration*>(node)) {
//          addIdentifier(functionDeclaration->identifier->name, functionDeclaration);
        }

        else if (auto* classDeclaration = dynamic_cast<const SyntaxNodeClassDeclaration*>(node)) {
//          addIdentifier(classDeclaration->identifier->name, classDeclaration);
        }

        else if (auto* observerDeclaration = dynamic_cast<const SyntaxNodeObserverDeclaration*>(node)) {
//          std::cout << "Observer Declaration: " << observerDeclaration->output() << std::endl;
//          runASTNode(observerDeclaration->identifier);
//          for (auto &argument : observerDeclaration->arguments) {
//              runASTNode(argument);
//          }
        }
        
        return {};
    }

    void ASTRunner::runAST(const AbstractSyntaxTree &ast) {
        for (auto* node : ast.root()) {
            ASTRunner* runner = new ASTRunner();
            runner->runASTNode(node);

            delete runner;
        }
    }
}
