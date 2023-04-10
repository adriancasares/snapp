#include "syntax/abstractSyntaxTree.h"
#include "error/syntaxError.h"

namespace Snapp {

    namespace {

        SyntaxNodeBlockStatement* getBodyBlockStatement(SyntaxNode* body) {
            if (auto* block = dynamic_cast<SyntaxNodeBlockStatement*>(body)) {
                return block;
            } else {
                throw SyntaxError("expected a block of statements");
            }
        }

        SyntaxNode* parseExpression(TokenIterator& iter, const TokenIterator& end) {
            return nullptr;
        }

        SyntaxNode* parseStatement(TokenIterator& iter, const TokenIterator& end) {
            while (iter != end && iter->has(Symbol::Semicolon)) {
                ++iter;
            }
            if (iter == end) {
                return nullptr;
            }
            if (iter->has(Keyword::Class)) {
                ++iter;
                const std::string& name = iter->expectIdentifier().name;
                ++iter;
                SyntaxNode* body = parseStatement(iter, end);
            }
            return nullptr; // temporary
        }

    }

    AbstractSyntaxTree::~AbstractSyntaxTree() {
        for (SyntaxNode* node : root_) {
            delete node;
        }
    }

    const std::vector<SyntaxNode*>& AbstractSyntaxTree::root() const {
        return root_;
    }

    AbstractSyntaxTree AbstractSyntaxTree::fromTokens(const std::vector<Token>& tokens) {
        TokenIterator iter = tokens.begin(), end = tokens.end();
        AbstractSyntaxTree tree;
        while (iter != end) {
            if (SyntaxNode* statement = parseStatement(iter, end)) {
                tree.root_.push_back(statement);
            }
        }
        return tree;
    }

    std::ostream& operator<<(std::ostream& out, const AbstractSyntaxTree& tree) {
        return out << "AbstractSyntaxTree (TODO)"; // TODO
    }

}