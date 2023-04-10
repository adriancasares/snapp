#include "syntax/abstractSyntaxTree.h"

namespace {
    // TODO: utility functions here
}

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

    //AbstractSyntaxTree AbstractSyntaxTree::fromTokens(const std::vector<Token>& tokens) {}

}