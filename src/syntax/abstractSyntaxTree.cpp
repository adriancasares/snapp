#include "syntax/abstractSyntaxTree.h"
#include "error/syntaxError.h"

namespace Snapp {

    namespace {

        enum class StopperRule {
            Semicolon,
            Comma,
            ParenRight,
            Colon,
            CommaOrParenRight,
        };

        bool shouldUseToken(const TokenIterator& iter, const TokenIterator& end, StopperRule stopper) {
            if (iter == end) {
                return false;
            }
            switch (stopper) {
                case StopperRule::Semicolon:
                    return !iter->has(Symbol::Semicolon);
                case StopperRule::Comma:
                    return !iter->has(Symbol::Comma);
                case StopperRule::ParenRight:
                    return !iter->has(Symbol::ParenRight);
                case StopperRule::Colon:
                    return !iter->has(Symbol::Colon);
                case StopperRule::CommaOrParenRight:
                    return !iter->has(Symbol::Comma) && !iter->has(Symbol::ParenRight);
                default:
                    return true;
            }
        }

        void nextToken(TokenIterator& iter, const TokenIterator& end) {
            ++iter;
            if (iter == end) {
                --iter;
                throw SyntaxError("unexpectedly reached end of file", iter->end(), {});
            }
        }

        SyntaxNode* parseStatement(TokenIterator& iter, const TokenIterator& end, bool required = true);

        SyntaxNode* parseExpression(TokenIterator& iter, const TokenIterator& end, StopperRule stopper) {
            return nullptr; // temporary
        }

        SyntaxNode* tryParseDeclaration(TokenIterator& iter, const TokenIterator& end, StopperRule stopper, bool functionAllowed = false, bool isPrivate = false) {
            TokenIterator start(iter);
            bool liveType = false;
            if (iter->has(Symbol::AtSign)) {
                liveType = true;
                nextToken(iter, end);
            }
            if (std::holds_alternative<Identifier>(iter->value())) {
                const std::string& typeName = std::get<Identifier>(iter->value()).name;
                nextToken(iter, end);
                if (std::holds_alternative<Identifier>(iter->value())) {
                    DataType dataType(findBaseDataType(typeName), typeName, liveType);
                    const std::string& name = std::get<Identifier>(iter->value()).name;
                    nextToken(iter, end);
                    if (iter->has(Symbol::Equal)) {
                        nextToken(iter, end);
                        auto* value = parseExpression(iter, end, stopper);
                        return new SyntaxNodeVariableDeclaration(dataType, new SyntaxNodeIdentifier(name), value, isPrivate);
                    }
                    else if (functionAllowed && iter->has(Symbol::ParenLeft)) {
                        nextToken(iter, end);
                        std::vector<SyntaxNodeVariableDeclaration*> parameters;
                        while (!iter->has(Symbol::ParenRight)) {
                            if (auto* parameter = dynamic_cast<SyntaxNodeVariableDeclaration*>(tryParseDeclaration(iter, end, StopperRule::CommaOrParenRight))) {
                                parameters.push_back(parameter);
                            } else {
                                throw SyntaxError("invalid function parameter", iter->start(), iter->start());
                            }
                            if (iter->has(Symbol::Comma)) {
                                nextToken(iter, end);
                            }
                        }
                        nextToken(iter, end);
                        iter->expect(Symbol::CurlyLeft);
                        auto* body = parseStatement(iter, end);
                        return new SyntaxNodeFunctionDeclaration(dataType, new SyntaxNodeIdentifier(name), parameters, body, isPrivate);
                    }
                    else if (shouldUseToken(iter, end, stopper)) {
                        throw SyntaxError("unexpected token in declaration", iter->start(), iter->end());
                    }
                    return new SyntaxNodeVariableDeclaration(dataType, new SyntaxNodeIdentifier(name), nullptr, isPrivate);
                }
            }
            iter = start;
            return nullptr;
        }

        SyntaxNode* parseStatement(TokenIterator& iter, const TokenIterator& end, bool required) {
            if (iter == end || iter->has(Symbol::Semicolon)) {
                if (required) {
                    --iter;
                    throw SyntaxError("expected a statement", iter->end(), {});
                }
                return nullptr;
            }
            else if (iter->has(Symbol::CurlyLeft)) {
                nextToken(iter, end);
                auto* block = new SyntaxNodeBlockStatement({});
                while (!iter->has(Symbol::CurlyRight)) {
                    if (auto* statement = parseStatement(iter, end, false)) {
                        block->statements.push_back(statement);
                    }
                    nextToken(iter, end);
                }
                if (block->statements.empty()) {
                    delete block;
                    return nullptr;
                }
                else if (block->statements.size() == 1) {
                    auto* statement = block->statements[0];
                    delete block;
                    return statement;
                }
                return block;
            }
            bool isPrivate = false, visibilitySpecified = false;
            if (iter->has(Keyword::Private)) {
                isPrivate = true;
                visibilitySpecified = true;
                nextToken(iter, end);
            }
            else if (iter->has(Keyword::Public)) {
                visibilitySpecified = true;
                nextToken(iter, end);
            }
            if (iter->has(Keyword::Class)) {
                nextToken(iter, end);
                const std::string& name = iter->expectIdentifier().name;
                nextToken(iter, end);
                iter->expect(Symbol::CurlyLeft);
                auto* body = parseStatement(iter, end);
                return new SyntaxNodeClassDeclaration(new SyntaxNodeIdentifier(name), body, isPrivate);
            }
            else if (auto* declaration = tryParseDeclaration(iter, end, StopperRule::Semicolon, true, isPrivate)) {
                return declaration;
            }
            else if (visibilitySpecified) {
                --iter;
                throw SyntaxError(std::string("'") + (isPrivate ? "private" : "public") + "' can only apply to variable, function, or class declarations", iter->start(), iter->end());
            }
            else if (iter->has(Keyword::Observe)) {
                nextToken(iter, end);
                const std::string& name = iter->expectIdentifier().name;
                nextToken(iter, end);
                iter->expect(Keyword::As);
                nextToken(iter, end);
                const std::string& alias = iter->expectIdentifier().name;
                nextToken(iter, end);
                iter->expect(Symbol::CurlyLeft);
                auto* body = parseStatement(iter, end);
                return new SyntaxNodeObserverDeclaration(new SyntaxNodeIdentifier(name), new SyntaxNodeIdentifier(alias), body);
            }
            else if (iter->has(Keyword::If)) {
                nextToken(iter, end);
                iter->expect(Symbol::ParenLeft);
                nextToken(iter, end);
                auto* condition = tryParseDeclaration(iter, end, StopperRule::ParenRight);
                if (!condition) {
                    condition = parseExpression(iter, end, StopperRule::ParenRight);
                }
                nextToken(iter, end);
                auto* consequent = parseStatement(iter, end);
                ++iter;
                SyntaxNode* alternative = nullptr;
                if (iter != end && iter->has(Keyword::Else)) {
                    nextToken(iter, end);
                    alternative = parseStatement(iter, end);
                } else {
                    --iter;
                }
                return new SyntaxNodeIfStatement(condition, consequent, alternative);
            }
            else if (iter->has(Keyword::While)) {
                nextToken(iter, end);
                iter->expect(Symbol::ParenLeft);
                nextToken(iter, end);
                auto* condition = tryParseDeclaration(iter, end, StopperRule::ParenRight);
                if (!condition) {
                    condition = parseExpression(iter, end, StopperRule::ParenRight);
                }
                nextToken(iter, end);
                auto* loop = parseStatement(iter, end);
                return new SyntaxNodeWhileStatement(condition, loop);
            }
            else if (iter->has(Keyword::For)) {
                nextToken(iter, end);
                iter->expect(Symbol::ParenLeft);
                nextToken(iter, end);
                auto* initialize = tryParseDeclaration(iter, end, StopperRule::Semicolon);
                if (!initialize && !iter->has(Symbol::Semicolon)) {
                    initialize = parseExpression(iter, end, StopperRule::Semicolon);
                }
                nextToken(iter, end);
                auto* condition = tryParseDeclaration(iter, end, StopperRule::Semicolon);
                if (!condition && !iter->has(Symbol::Semicolon)) {
                    condition = parseExpression(iter, end, StopperRule::Semicolon);
                }
                nextToken(iter, end);
                auto* update = iter->has(Symbol::ParenRight) ? nullptr : parseExpression(iter, end, StopperRule::ParenRight);
                nextToken(iter, end);
                auto* loop = parseStatement(iter, end);
                return new SyntaxNodeForStatement(initialize, condition, update, loop);
            }
            else if (iter->has(Keyword::Return)) {
                nextToken(iter, end);
                auto* value = parseExpression(iter, end, StopperRule::Semicolon);
                return new SyntaxNodeReturnStatement(value);
            }
            else {
                return parseExpression(iter, end, StopperRule::Semicolon);
            }
        }

    }

    AbstractSyntaxTree::~AbstractSyntaxTree() {
        for (auto* node : root_) {
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
            if (auto* statement = parseStatement(iter, end, false)) {
                tree.root_.push_back(statement);
            }
            ++iter;
        }
        return tree;
    }

    std::ostream& operator<<(std::ostream& out, const AbstractSyntaxTree& tree) {
        return out << "AbstractSyntaxTree (TODO)"; // TODO
    }

}