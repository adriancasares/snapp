#ifndef SYNTAX_SYNTAXNODE_H_
#define SYNTAX_SYNTAXNODE_H_

#include <iostream>
#include <vector>
#include <variant>

#include "dataType.h"
#include "operation.h"
#include "token.h"

namespace Snapp {

    class SyntaxNode {
    public:
        virtual ~SyntaxNode() = default;
    };

    class SyntaxNodeLiteral : public SyntaxNode {
    public:
        using Value = std::variant<int, double, bool, std::string>;
        SyntaxNodeLiteral(Value value);
        ~SyntaxNodeLiteral() = default;
        Value value;
    };

    class SyntaxNodeIdentifier : public SyntaxNode {
    public:
        SyntaxNodeIdentifier(std::string name);
        ~SyntaxNodeIdentifier() = default;
        std::string name;
    };

    class SyntaxNodeUnaryExpression : public SyntaxNode {
    public:
        SyntaxNodeUnaryExpression(Operation operation, SyntaxNode* operand);
        ~SyntaxNodeUnaryExpression();
        Operation operation;
        SyntaxNode* operand;
    };

    class SyntaxNodeBinaryExpression : public SyntaxNode {
    public:
        SyntaxNodeBinaryExpression(Operation operation, SyntaxNode* leftSide, SyntaxNode* rightSide);
        ~SyntaxNodeBinaryExpression();
        Operation operation;
        SyntaxNode* leftSide;
        SyntaxNode* rightSide;
    };

    class SyntaxNodeFunctionCall : public SyntaxNode {
    public:
        SyntaxNodeFunctionCall(SyntaxNodeIdentifier* identifier, std::vector<SyntaxNode*> arguments);
        ~SyntaxNodeFunctionCall();
        SyntaxNodeIdentifier* identifier;
        std::vector<SyntaxNode*> arguments;
    };

    class SyntaxNodeVariableDeclaration : public SyntaxNode {
    public:
        SyntaxNodeVariableDeclaration(DataType dataType, SyntaxNodeIdentifier* identifier, SyntaxNode* value, bool isPrivate = false);
        ~SyntaxNodeVariableDeclaration();
        DataType dataType;
        SyntaxNodeIdentifier* identifier;
        SyntaxNode* value;
        bool isPrivate;
    };

    class SyntaxNodeBlockStatement : public SyntaxNode {
    public:
        SyntaxNodeBlockStatement(std::vector<SyntaxNode*> statements);
        ~SyntaxNodeBlockStatement();
        std::vector<SyntaxNode*> statements;
    };

    class SyntaxNodeIfStatement : public SyntaxNode {
    public:
        SyntaxNodeIfStatement(SyntaxNode* condition, SyntaxNode* consequent, SyntaxNode* alternative);
        ~SyntaxNodeIfStatement();
        SyntaxNode* condition;
        SyntaxNode* consequent;
        SyntaxNode* alternative;
    };

    class SyntaxNodeWhileStatement : public SyntaxNode {
    public:
        SyntaxNodeWhileStatement(SyntaxNode* condition, SyntaxNode* loop);
        ~SyntaxNodeWhileStatement();
        SyntaxNode* condition;
        SyntaxNode* loop;
    };

    class SyntaxNodeForStatement : public SyntaxNode {
    public:
        SyntaxNodeForStatement(SyntaxNode* initialize, SyntaxNode* condition, SyntaxNode* update, SyntaxNode* loop);
        ~SyntaxNodeForStatement();
        SyntaxNode* initialize;
        SyntaxNode* condition;
        SyntaxNode* update;
        SyntaxNode* loop;
    };

    class SyntaxNodeReturnStatement : public SyntaxNode {
    public:
        SyntaxNodeReturnStatement(SyntaxNode* value);
        ~SyntaxNodeReturnStatement();
        SyntaxNode* value;
    };

    class SyntaxNodeFunctionDeclaration : public SyntaxNode {
    public:
        SyntaxNodeFunctionDeclaration(DataType returnType, SyntaxNodeIdentifier* identifier, std::vector<SyntaxNodeVariableDeclaration*> parameters, SyntaxNode* body, bool isPrivate = false);
        ~SyntaxNodeFunctionDeclaration();
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
        SyntaxNodeIdentifier* identifier;
        SyntaxNode* body;
        bool isPrivate;
    };

    class SyntaxNodeObserverDeclaration : public SyntaxNode {
    public:
        SyntaxNodeObserverDeclaration(SyntaxNodeIdentifier* identifier, SyntaxNodeIdentifier* alias, SyntaxNode* body);
        ~SyntaxNodeObserverDeclaration();
        SyntaxNodeIdentifier* identifier;
        SyntaxNodeIdentifier* alias;
        SyntaxNode* body;
    };

}

#endif /* SYNTAX_SYNTAXNODE_H_ */
