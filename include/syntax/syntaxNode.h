//
// Created by Adrian Casares on 4/7/23.
//

#ifndef SYNTAX_SYNTAXNODE_H_
#define SYNTAX_SYNTAXNODE_H_

#include <iostream>
#include <vector>

namespace Snapp {

  class SyntaxNode {
    public:
      SyntaxNode() = default;
      virtual ~SyntaxNode() = default;
  };

  class SyntaxNodeLiteral : public SyntaxNode {
    public:
      SyntaxNodeLiteral() = default;
      virtual ~SyntaxNodeLiteral() = default;
      std::variant<int, float, bool, std::string> literal;
  };

    class SyntaxNodeIdentifier : public SyntaxNode {
        public:
        SyntaxNodeIdentifier() = default;
        virtual ~SyntaxNodeIdentifier() = default;
        std::string identifier;
    };

    class SyntaxNodeMemberAccess : public SyntaxNode {
        public:
        SyntaxNodeMemberAccess() = default;
        virtual ~SyntaxNodeMemberAccess() = default;
        SyntaxNodeIdentifier* objectNode;
        std::string property;
    };

    class SyntaxNodeBinaryExpression : public SyntaxNode {
        public:
        SyntaxNodeBinaryExpression() = default;
        virtual ~SyntaxNodeBinaryExpression() = default;
        SyntaxNode* leftNode;
        SyntaxNode* rightNode;
        std::string operator_;
    };

    class SyntaxNodeUnaryExpression : public SyntaxNode {
        public:
        SyntaxNodeUnaryExpression() = default;
        virtual ~SyntaxNodeUnaryExpression() = default;
        SyntaxNode* node;
        std::string operator_;
    };

    class SyntaxNodeFunctionCall : public SyntaxNode {
        public:
        SyntaxNodeFunctionCall() = default;
        virtual ~SyntaxNodeFunctionCall() = default;
        SyntaxNodeIdentifier* functionNode;
        std::vector<SyntaxNode*> arguments;
    };

    class SyntaxNodeFunctionDeclaration : public SyntaxNode {
        public:
        SyntaxNodeFunctionDeclaration() = default;
        virtual ~SyntaxNodeFunctionDeclaration() = default;
        SyntaxNodeIdentifier* identifierNode;
        std::vector<SyntaxNodeIdentifier*> arguments;
        std::vector<SyntaxNode*> statements;
    };

    class SyntaxNodeClassDeclaration : public SyntaxNode {
        public:
        SyntaxNodeClassDeclaration() = default;
        virtual ~SyntaxNodeClassDeclaration() = default;
        SyntaxNodeIdentifier* identifierNode;
        std::vector<SyntaxNode*> statements;
    };

    class SyntaxNodeIfStatement : public SyntaxNode {
        public:
        SyntaxNodeIfStatement() = default;
        virtual ~SyntaxNodeIfStatement() = default;
        SyntaxNode* conditionNode;
        std::vector<SyntaxNode*> statements;
        std::vector<SyntaxNode*> elseStatements;
    };

    class SyntaxNodeWhileStatement : public SyntaxNode {
        public:
        SyntaxNodeWhileStatement() = default;
        virtual ~SyntaxNodeWhileStatement() = default;
        SyntaxNode* conditionNode;
        std::vector<SyntaxNode*> statements;
    };

    class SyntaxNodeReturnStatement : public SyntaxNode {
        public:
        SyntaxNodeReturnStatement() = default;
        virtual ~SyntaxNodeReturnStatement() = default;
        SyntaxNode* node;
    };

    class SyntaxNodeVariableDeclaration : public SyntaxNode {
        public:
        SyntaxNodeVariableDeclaration() = default;
        virtual ~SyntaxNodeVariableDeclaration() = default;
        SyntaxNodeIdentifier* identifierNode;
        SyntaxNode* valueNode;
    };






}

#endif //SNAPP_INCLUDE_SYNTAX_SYNTAXNODE_H_
