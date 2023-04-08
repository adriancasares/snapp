//
// Created by Adrian Casares on 4/7/23.
//

#ifndef SYNTAX_ABSTRACTSYNTAXTREE_H_
#define SYNTAX_ABSTRACTSYNTAXTREE_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "syntaxNode.h"

namespace Snapp {

    class AbstractSyntaxTree {
    public:
        AbstractSyntaxTree();
        virtual ~AbstractSyntaxTree();

        void setRoot(SyntaxNode* root);
        SyntaxNode* getRoot() const;

    private:
        SyntaxNode* root;
    };

}

#endif /* SYNTAX_ABSTRACTSYNTAXTREE_H_ */
