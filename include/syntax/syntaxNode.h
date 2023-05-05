#ifndef SNAPP_SYNTAX_SYNTAXNODE_H_
#define SNAPP_SYNTAX_SYNTAXNODE_H_

#include <iostream>
#include <vector>
#include <variant>

#include "operation.h"
#include "value/dataType.h"
#include "value/dataValue.h"
#include "value/function.h"
#include "value/class.h"

namespace Snapp {

    class SyntaxNode {
    public:
        virtual ~SyntaxNode() = default;
        virtual std::string output() const = 0;
    };

    class SyntaxNodeLiteral : public SyntaxNode {
    public:
        SyntaxNodeLiteral(DataValue value);
        ~SyntaxNodeLiteral() = default;
        std::string output() const;
        DataValue value;
    };

    class SyntaxNodeIdentifier : public SyntaxNode {
    public:
        SyntaxNodeIdentifier(std::string name);
        ~SyntaxNodeIdentifier() = default;
        std::string output() const;
        std::string name;
    };

    class SyntaxNodeUnaryExpression : public SyntaxNode {
    public:
        SyntaxNodeUnaryExpression(Operation operation, SyntaxNode* operand);
        ~SyntaxNodeUnaryExpression();
        std::string output() const;
        Operation operation;
        SyntaxNode* operand;
    };

    class SyntaxNodeBinaryExpression : public SyntaxNode {
    public:
        SyntaxNodeBinaryExpression(Operation operation, SyntaxNode* leftSide, SyntaxNode* rightSide);
        ~SyntaxNodeBinaryExpression();
        std::string output() const;
        Operation operation;
        SyntaxNode* leftSide;
        SyntaxNode* rightSide;
    };

    class SyntaxNodeFunctionCall : public SyntaxNode {
    public:
        SyntaxNodeFunctionCall(SyntaxNode* callee, std::vector<SyntaxNode*> arguments);
        ~SyntaxNodeFunctionCall();
        std::string output() const;
        SyntaxNode* callee;
        std::vector<SyntaxNode*> arguments;
    };

    class SyntaxNodeVariableDeclaration : public SyntaxNode {
    public:
        SyntaxNodeVariableDeclaration(DataType dataType, SyntaxNodeIdentifier* identifier, SyntaxNode* value, bool isPrivate = false);
        ~SyntaxNodeVariableDeclaration();
        std::string output() const;
        DataType dataType;
        SyntaxNodeIdentifier* identifier;
        SyntaxNode* value;
        bool isPrivate;
    };

    class SyntaxNodeBlockStatement : public SyntaxNode {
    public:
        SyntaxNodeBlockStatement(std::vector<SyntaxNode*> statements);
        ~SyntaxNodeBlockStatement();
        std::string output() const;
        std::vector<SyntaxNode*> statements;
    };

    class SyntaxNodeIfStatement : public SyntaxNode {
    public:
        SyntaxNodeIfStatement(SyntaxNode* condition, SyntaxNode* consequent, SyntaxNode* alternative);
        ~SyntaxNodeIfStatement();
        std::string output() const;
        SyntaxNode* condition;
        SyntaxNode* consequent;
        SyntaxNode* alternative;
    };

    class SyntaxNodeWhileStatement : public SyntaxNode {
    public:
        SyntaxNodeWhileStatement(SyntaxNode* condition, SyntaxNode* loop);
        ~SyntaxNodeWhileStatement();
        std::string output() const;
        SyntaxNode* condition;
        SyntaxNode* loop;
    };

    class SyntaxNodeForStatement : public SyntaxNode {
    public:
        SyntaxNodeForStatement(SyntaxNode* initialize, SyntaxNode* condition, SyntaxNode* update, SyntaxNode* loop);
        ~SyntaxNodeForStatement();
        std::string output() const;
        SyntaxNode* initialize;
        SyntaxNode* condition;
        SyntaxNode* update;
        SyntaxNode* loop;
    };

    class SyntaxNodeReturnStatement : public SyntaxNode {
    public:
        SyntaxNodeReturnStatement(SyntaxNode* value);
        ~SyntaxNodeReturnStatement();
        std::string output() const;
        SyntaxNode* value;
    };

    class SyntaxNodeFunctionDeclaration : public SyntaxNode {
    public:
        SyntaxNodeFunctionDeclaration(DataType returnType, SyntaxNodeIdentifier* identifier, std::vector<SyntaxNodeVariableDeclaration*> parameters, SyntaxNode* body, bool isPrivate = false);
        ~SyntaxNodeFunctionDeclaration();
        std::string output() const;
        DataType returnType;
        SyntaxNodeIdentifier* identifier;
        std::vector<SyntaxNodeVariableDeclaration*> parameters;
        SyntaxNode* body;
        bool isPrivate;
    };

    class SyntaxNodeClassDeclaration : public SyntaxNode {
    public:
        SyntaxNodeClassDeclaration(SyntaxNodeIdentifier* identifier, SyntaxNode* body, bool isPrivate = false);
        ~SyntaxNodeClassDeclaration();
        std::string output() const;
        SyntaxNodeIdentifier* identifier;
        SyntaxNode* body;
        bool isPrivate;
    };

    class SyntaxNodeObserverDeclaration : public SyntaxNode {
    public:
        SyntaxNodeObserverDeclaration(SyntaxNodeIdentifier* identifier, SyntaxNodeIdentifier* alias, SyntaxNode* body);
        ~SyntaxNodeObserverDeclaration();
        std::string output() const;
        SyntaxNodeIdentifier* identifier;
        SyntaxNodeIdentifier* alias;
        SyntaxNode* body;
    };

}

#endif /* SNAPP_SYNTAX_SYNTAXNODE_H_ */
