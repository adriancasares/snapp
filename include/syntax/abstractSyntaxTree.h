#ifndef SYNTAX_ABSTRACTSYNTAXTREE_H_
#define SYNTAX_ABSTRACTSYNTAXTREE_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "syntaxNode.h"
#include "token.h"

namespace Snapp {

    class AbstractSyntaxTree {
    public:
        AbstractSyntaxTree();
        virtual ~AbstractSyntaxTree();

        void setRoot(SyntaxNode* root);
        SyntaxNode* getRoot() const;

        static AbstractSyntaxTree fromTokens(const std::vector<Token>& tokens);

    private:
        SyntaxNode* root;
    };

}

#endif /* SYNTAX_ABSTRACTSYNTAXTREE_H_ */
