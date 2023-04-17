#include "syntax/syntaxNode.h"

#include <sstream>

namespace Snapp {

    SyntaxNodeLiteral::SyntaxNodeLiteral(Value value) :
        value(value) {}

    std::string SyntaxNodeLiteral::output() const {
        if (const int* i = std::get_if<int>(&value)) {
            return "int(" + std::to_string(*i) + ")";
        }
        else if (const double* d = std::get_if<double>(&value)) {
            return "float(" + std::to_string(*d) + ")";
        }
        else if (const bool* b = std::get_if<bool>(&value)) {
            return *b ? "bool(true)" : "bool(false)";
        }
        else if (const std::string* s = std::get_if<std::string>(&value)) {
            return "str(\"" + *s + "\")";
        }
        else {
            return "<unknown>";
        }
    }

    SyntaxNodeIdentifier::SyntaxNodeIdentifier(std::string name) :
        name(name) {}

    std::string SyntaxNodeIdentifier::output() const {
        return "id(" + name + ")";
    }

    SyntaxNodeUnaryExpression::SyntaxNodeUnaryExpression(Operation operation, SyntaxNode* operand) :
        operation(operation),
        operand(operand) {}

    SyntaxNodeUnaryExpression::~SyntaxNodeUnaryExpression() {
        delete operand;
    }

    std::string SyntaxNodeUnaryExpression::output() const {
        std::ostringstream out;
        out << operationNames.at(operation) << "(" << operand->output() << ")";
        return out.str();
    }

    SyntaxNodeBinaryExpression::SyntaxNodeBinaryExpression(Operation operation, SyntaxNode* leftSide, SyntaxNode* rightSide) :
        operation(operation),
        leftSide(leftSide),
        rightSide(rightSide) {}

    SyntaxNodeBinaryExpression::~SyntaxNodeBinaryExpression() {
        delete leftSide;
        delete rightSide;
    }

    std::string SyntaxNodeBinaryExpression::output() const {
        std::ostringstream out;
        out << operationNames.at(operation) << "(" << leftSide->output() << ", " << rightSide->output() << ")";
        return out.str();
    }

    SyntaxNodeFunctionCall::SyntaxNodeFunctionCall(SyntaxNode* callee, std::vector<SyntaxNode*> arguments) :
        callee(callee),
        arguments(arguments) {}

    SyntaxNodeFunctionCall::~SyntaxNodeFunctionCall() {
        delete callee;
        for (auto* argument : arguments) {
            delete argument;
        }
    }

    std::string SyntaxNodeFunctionCall::output() const {
        std::ostringstream out;
        out << "Call[" << callee->output() << "](";
        for (int i = 0; i < arguments.size(); ++i) {
            if (i != 0) {
                out << ", ";
            }
            out << arguments[i]->output();
        }
        out << ")";
        return out.str();
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

    std::string SyntaxNodeVariableDeclaration::output() const {
        std::ostringstream out;
        out << "var[";
        if (isPrivate) {
            out << "private, ";
        }
        out << dataType << ", " << identifier->output() << "]";
        if (value) {
            out << "(" << value->output() << ")";
        }
        return out.str();
    }

    SyntaxNodeBlockStatement::SyntaxNodeBlockStatement(std::vector<SyntaxNode*> statements) :
        statements(statements) {}

    SyntaxNodeBlockStatement::~SyntaxNodeBlockStatement() {
        for (auto* statement : statements) {
            delete statement;
        }
    }

    std::string SyntaxNodeBlockStatement::output() const {
        std::ostringstream out;
        out << "block{";
        for (int i = 0; i < statements.size(); ++i) {
            if (i != 0) {
                out << "; ";
            }
            if (statements[i]) {
                out << statements[i]->output();
            }
        }
        out << "}";
        return out.str();
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

    std::string SyntaxNodeIfStatement::output() const {
        std::ostringstream out;
        out << "if(" << condition->output() << "){";
        if (consequent) {
            out << consequent->output();
        }
        out << "}";
        if (alternative) {
            out << "else{" << alternative->output() << "}";
        }
        return out.str();
    }

    SyntaxNodeWhileStatement::SyntaxNodeWhileStatement(SyntaxNode* condition, SyntaxNode* loop) :
        condition(condition),
        loop(loop) {}

    SyntaxNodeWhileStatement::~SyntaxNodeWhileStatement() {
        delete condition;
        delete loop;
    }

    std::string SyntaxNodeWhileStatement::output() const {
        std::ostringstream out;
        out << "while(" << condition->output() << "){";
        if (loop) {
            out << loop->output();
        }
        out << "}";
        return out.str();
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

    std::string SyntaxNodeForStatement::output() const {
        std::ostringstream out;
        out << "for(";
        if (initialize) {
            out << initialize->output();
        }
        out << "; ";
        if (condition) {
            out << condition->output();
        }
        out << "; ";
        if (update) {
            out << update->output();
        }
        out << "){" << loop->output() << "}";
        return out.str();
    }

    SyntaxNodeReturnStatement::SyntaxNodeReturnStatement(SyntaxNode* value) :
        value(value) {}

    SyntaxNodeReturnStatement::~SyntaxNodeReturnStatement() {
        delete value;
    }

    std::string SyntaxNodeReturnStatement::output() const {
        std::ostringstream out;
        out << "return(" << value->output() << ")";
        return out.str();
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

    std::string SyntaxNodeFunctionDeclaration::output() const {
        std::ostringstream out;
        out << "fn[";
        if (isPrivate) {
            out << "private, ";
        }
        out << returnType << ", " << identifier->output() << "](";
        for (int i = 0; i < parameters.size(); ++i) {
            if (i != 0) {
                out << ", ";
            }
            out << parameters[i]->output();
        }
        out << "){";
        if (body) {
            out << body->output();
        }
        out << "}";
        return out.str();
    }

    SyntaxNodeClassDeclaration::SyntaxNodeClassDeclaration(SyntaxNodeIdentifier* identifier, SyntaxNode* body, bool isPrivate) :
        identifier(identifier),
        body(body),
        isPrivate(isPrivate) {}

    SyntaxNodeClassDeclaration::~SyntaxNodeClassDeclaration() {
        delete identifier;
        delete body;
    }

    std::string SyntaxNodeClassDeclaration::output() const {
        std::ostringstream out;
        out << "class[";
        if (isPrivate) {
            out << "private, ";
        }
        out << identifier->output() << "]{";
        if (body) {
            out << body->output();
        }
        out << "}";
        return out.str();
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

    std::string SyntaxNodeObserverDeclaration::output() const {
        std::ostringstream out;
        out << "observe(" << identifier->output() << ", " << alias->output() << "){";
        if (body) {
            out << body->output();
        }
        out << "}";
        return out.str();
    }

}