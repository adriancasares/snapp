//
// Created by Adrian Casares on 4/17/23.
//
// function to run the AST

#include <cmath>
#include "semantics/astRunner.h"

namespace Snapp {

    namespace {

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
            auto it = identifiers.find(identifier->name);
            if (it != identifiers.end()) {
                return it->second;
            } else {
                return {}; // TODO error
            }
        }

        else if (auto* unaryExpression = dynamic_cast<const SyntaxNodeUnaryExpression*>(node)) {
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

        else if (auto* binaryExpression = dynamic_cast<const SyntaxNodeBinaryExpression*>(node)) {
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
                DataValue left = *runASTNode(binaryExpression->leftSide);
                DataValue right = *runASTNode(binaryExpression->rightSide);
                // TODO
                return right;
            }
        }

        else if (auto* functionCall = dynamic_cast<const SyntaxNodeFunctionCall*>(node)) {
            DataValue callee = *runASTNode(functionCall->callee);
            for (auto* argument : functionCall->arguments) {
                runASTNode(argument);
            }
        }

        else if (auto* variableDeclaration = dynamic_cast<const SyntaxNodeVariableDeclaration*>(node)) {
            DataValue value = *runASTNode(variableDeclaration->value);

            std::cout << "Variable (" << variableDeclaration->dataType << " " << variableDeclaration->identifier->name << "): " << *coerceStr(value) << std::endl;
            addIdentifier(variableDeclaration->identifier->name, value);
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
            ASTRunner runner;
            runner.runASTNode(node);
        }
    }
}
