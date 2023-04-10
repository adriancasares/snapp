#include "syntax/syntaxNode.h"

namespace Snapp {

    SyntaxNodeMemberAccess::~SyntaxNodeMemberAccess() {
        delete container;
        delete property;
    }

    SyntaxNodeUnaryExpression::~SyntaxNodeUnaryExpression() {
        delete operand;
    }

    SyntaxNodeBinaryExpression::~SyntaxNodeBinaryExpression() {
        delete leftSide;
        delete rightSide;
    }

    SyntaxNodeFunctionCall::~SyntaxNodeFunctionCall() {
        delete identifier;
        for (SyntaxNode* argument : arguments) {
            delete argument;
        }
    }

    SyntaxNodeVariableDeclaration::~SyntaxNodeVariableDeclaration() {
        delete identifier;
        delete value;
    }

    SyntaxNodeBlockStatement::~SyntaxNodeBlockStatement() {
        for (SyntaxNode* statement : statements) {
            delete statement;
        }
    }

    SyntaxNodeIfStatement::~SyntaxNodeIfStatement() {
        delete condition;
        delete consequent;
        delete alternative;
    }

    SyntaxNodeWhileStatement::~SyntaxNodeWhileStatement() {
        delete condition;
        delete loop;
    }

    SyntaxNodeForStatement::~SyntaxNodeForStatement() {
        delete initialize;
        delete condition;
        delete update;
        delete loop;
    }

    SyntaxNodeReturnStatement::~SyntaxNodeReturnStatement() {
        delete value;
    }

    SyntaxNodeFunctionDeclaration::~SyntaxNodeFunctionDeclaration() {
        delete identifier;
        for (SyntaxNodeVariableDeclaration* parameter : parameters) {
            delete parameter;
        }
        for (SyntaxNode* statement : statements) {
            delete statement;
        }
    }

    SyntaxNodeObserverDeclaration::~SyntaxNodeObserverDeclaration() {
        delete identifier;
        delete alias;
        for (SyntaxNode* statement : statements) {
            delete statement;
        }
    }

    SyntaxNodeClassDeclaration::~SyntaxNodeClassDeclaration() {
        delete identifier;
        for (SyntaxNode* member : members) {
            delete member;
        }
    }

}