#ifndef SYNTAX_SYNTAXNODE_H_
#define SYNTAX_SYNTAXNODE_H_

#include <iostream>
#include <vector>
#include <variant>

#include "dataType.h"
#include "token.h"

namespace Snapp {

    class SyntaxNode {
    public:
        virtual ~SyntaxNode() = default;
    };

    class SyntaxNodeLiteral : public SyntaxNode {
    public:
        ~SyntaxNodeLiteral() = default;
        std::variant<int, double, bool, std::string> value;
    };

    class SyntaxNodeIdentifier : public SyntaxNode {
    public:
        ~SyntaxNodeIdentifier() = default;
        std::string name;
    };

    class SyntaxNodeMemberAccess : public SyntaxNode {
    public:
        ~SyntaxNodeMemberAccess();
        SyntaxNode* container;
        SyntaxNodeIdentifier* property;
    };

    class SyntaxNodeUnaryExpression : public SyntaxNode {
    public:
        ~SyntaxNodeUnaryExpression();
        SyntaxNode* operand;
        std::string operation;
    };

    class SyntaxNodeBinaryExpression : public SyntaxNode {
    public:
        ~SyntaxNodeBinaryExpression();
        SyntaxNode* leftSide;
        SyntaxNode* rightSide;
        std::string operation;
    };

    class SyntaxNodeFunctionCall : public SyntaxNode {
    public:
        ~SyntaxNodeFunctionCall();
        SyntaxNodeIdentifier* identifier;
        std::vector<SyntaxNode*> arguments;
    };

    class SyntaxNodeVariableDeclaration : public SyntaxNode {
    public:
        ~SyntaxNodeVariableDeclaration();
        bool isPrivate;
        DataType dataType;
        SyntaxNodeIdentifier* identifier;
        SyntaxNode* value;
    };

    class SyntaxNodeBlockStatement : public SyntaxNode {
    public:
        ~SyntaxNodeBlockStatement();
        std::vector<SyntaxNode*> statements;
    };

    class SyntaxNodeIfStatement : public SyntaxNode {
    public:
        ~SyntaxNodeIfStatement();
        SyntaxNode* condition;
        SyntaxNode* consequent;
        SyntaxNode* alternative;
    };

    class SyntaxNodeWhileStatement : public SyntaxNode {
    public:
        ~SyntaxNodeWhileStatement();
        SyntaxNode* condition;
        SyntaxNode* loop;
    };

    class SyntaxNodeForStatement : public SyntaxNode {
    public:
        ~SyntaxNodeForStatement();
        SyntaxNode* initialize;
        SyntaxNode* condition;
        SyntaxNode* update;
        SyntaxNode* loop;
    };

    class SyntaxNodeReturnStatement : public SyntaxNode {
    public:
        ~SyntaxNodeReturnStatement();
        SyntaxNode* value;
    };

    class SyntaxNodeFunctionDeclaration : public SyntaxNode {
    public:
        ~SyntaxNodeFunctionDeclaration();
        bool isPrivate;
        DataType returnType;
        SyntaxNodeIdentifier* identifier;
        std::vector<SyntaxNodeVariableDeclaration*> parameters;
        SyntaxNodeBlockStatement* body;
    };

    class SyntaxNodeClassDeclaration : public SyntaxNode {
    public:
        ~SyntaxNodeClassDeclaration();
        bool isPrivate;
        SyntaxNodeIdentifier* identifier;
        SyntaxNodeBlockStatement* body;
    };

    class SyntaxNodeObserverDeclaration : public SyntaxNode {
    public:
        ~SyntaxNodeObserverDeclaration();
        SyntaxNodeIdentifier* identifier;
        SyntaxNodeIdentifier* alias;
        SyntaxNodeBlockStatement* body;
    };

}

#endif /* SYNTAX_SYNTAXNODE_H_ */
