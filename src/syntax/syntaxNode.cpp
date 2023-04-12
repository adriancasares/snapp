#include "syntax/syntaxNode.h"

namespace Snapp {

    SyntaxNodeLiteral::SyntaxNodeLiteral(Value value) :
        value(value) {}

    SyntaxNodeIdentifier::SyntaxNodeIdentifier(std::string name) :
        name(name) {}

    SyntaxNodeUnaryExpression::SyntaxNodeUnaryExpression(Operation operation, SyntaxNode* operand) :
        operation(operation),
        operand(operand) {}

    SyntaxNodeUnaryExpression::~SyntaxNodeUnaryExpression() {
        delete operand;
    }

    SyntaxNodeBinaryExpression::SyntaxNodeBinaryExpression(Operation operation, SyntaxNode* leftSide, SyntaxNode* rightSide) :
        operation(operation),
        leftSide(leftSide),
        rightSide(rightSide) {}

    SyntaxNodeBinaryExpression::~SyntaxNodeBinaryExpression() {
        delete leftSide;
        delete rightSide;
    }

    SyntaxNodeFunctionCall::SyntaxNodeFunctionCall(SyntaxNodeIdentifier* identifier, std::vector<SyntaxNode*> arguments) :
        identifier(identifier),
        arguments(arguments) {}

    SyntaxNodeFunctionCall::~SyntaxNodeFunctionCall() {
        delete identifier;
        for (auto* argument : arguments) {
            delete argument;
        }
    }

    SyntaxNodeVariableDeclaration::SyntaxNodeVariableDeclaration(DataType dataType, SyntaxNodeIdentifier* identifier, SyntaxNode* value, bool isPrivate) :
        dataType(dataType),
        identifier(identifier),
        value(value),
        isPrivate(isPrivate) {}

    SyntaxNodeVariableDeclaration::~SyntaxNodeVariableDeclaration() {
        delete identifier;
        delete value;
    }

    SyntaxNodeBlockStatement::SyntaxNodeBlockStatement(std::vector<SyntaxNode*> statements) :
        statements(statements) {}

    SyntaxNodeBlockStatement::~SyntaxNodeBlockStatement() {
        for (auto* statement : statements) {
            delete statement;
        }
    }

    SyntaxNodeIfStatement::SyntaxNodeIfStatement(SyntaxNode* condition, SyntaxNode* consequent, SyntaxNode* alternative) :
        condition(condition),
        consequent(consequent),
        alternative(alternative) {}

    SyntaxNodeIfStatement::~SyntaxNodeIfStatement() {
        delete condition;
        delete consequent;
        delete alternative;
    }

    SyntaxNodeWhileStatement::SyntaxNodeWhileStatement(SyntaxNode* condition, SyntaxNode* loop) :
        condition(condition),
        loop(loop) {}

    SyntaxNodeWhileStatement::~SyntaxNodeWhileStatement() {
        delete condition;
        delete loop;
    }

    SyntaxNodeForStatement::SyntaxNodeForStatement(SyntaxNode* initialize, SyntaxNode* condition, SyntaxNode* update, SyntaxNode* loop) :
        initialize(initialize),
        condition(condition),
        update(update),
        loop(loop) {}

    SyntaxNodeForStatement::~SyntaxNodeForStatement() {
        delete initialize;
        delete condition;
        delete update;
        delete loop;
    }

    SyntaxNodeReturnStatement::SyntaxNodeReturnStatement(SyntaxNode* value) :
        value(value) {}

    SyntaxNodeReturnStatement::~SyntaxNodeReturnStatement() {
        delete value;
    }

    SyntaxNodeFunctionDeclaration::SyntaxNodeFunctionDeclaration(DataType returnType, SyntaxNodeIdentifier* identifier, std::vector<SyntaxNodeVariableDeclaration*> parameters, SyntaxNode* body, bool isPrivate) :
        returnType(returnType),
        identifier(identifier),
        parameters(parameters),
        body(body),
        isPrivate(isPrivate) {}

    SyntaxNodeFunctionDeclaration::~SyntaxNodeFunctionDeclaration() {
        delete identifier;
        for (auto* parameter : parameters) {
            delete parameter;
        }
        delete body;
    }

    SyntaxNodeClassDeclaration::SyntaxNodeClassDeclaration(SyntaxNodeIdentifier* identifier, SyntaxNode* body, bool isPrivate) :
        identifier(identifier),
        body(body),
        isPrivate(isPrivate) {}

    SyntaxNodeClassDeclaration::~SyntaxNodeClassDeclaration() {
        delete identifier;
        delete body;
    }

    SyntaxNodeObserverDeclaration::SyntaxNodeObserverDeclaration(SyntaxNodeIdentifier* identifier, SyntaxNodeIdentifier* alias, SyntaxNode* body) :
        identifier(identifier),
        alias(alias),
        body(body) {}

    SyntaxNodeObserverDeclaration::~SyntaxNodeObserverDeclaration() {
        delete identifier;
        delete alias;
        delete body;
    }

}