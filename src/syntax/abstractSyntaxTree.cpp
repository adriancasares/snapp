//
// Created by Adrian Casares on 4/7/23.
//
#include "syntax/abstractSyntaxTree.h"

namespace Snapp {

    AbstractSyntaxTree::AbstractSyntaxTree() {
        root = nullptr;
    }

    AbstractSyntaxTree::~AbstractSyntaxTree() {
        delete root;
    }

    void AbstractSyntaxTree::setRoot(SyntaxNode* root) {
        this->root = root;
    }

    SyntaxNode* AbstractSyntaxTree::getRoot() const {
        return root;
    }

}