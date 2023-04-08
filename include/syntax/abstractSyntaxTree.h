//
// Created by Adrian Casares on 4/7/23.
//

#ifndef ABSTRACTSYNTAXTREE_H_
#define ABSTRACTSYNTAXTREE_H_

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
#endif
