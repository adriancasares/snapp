#ifndef SNAPP_SYNTAX_ABSTRACTSYNTAXTREE_H_
#define SNAPP_SYNTAX_ABSTRACTSYNTAXTREE_H_

#include <iostream>
#include <vector>

#include "syntaxNode.h"
#include "token.h"

namespace Snapp {

    class AbstractSyntaxTree {
    public:
        AbstractSyntaxTree() = default;
        ~AbstractSyntaxTree();

        const std::vector<SyntaxNode*>& root() const;

        static AbstractSyntaxTree fromTokens(const std::vector<Token>& tokens);

    private:
        std::vector<SyntaxNode*> root_;
    };

    std::ostream& operator<<(std::ostream& out, const AbstractSyntaxTree& tree);

}

#endif /* SNAPP_SYNTAX_ABSTRACTSYNTAXTREE_H_ */
