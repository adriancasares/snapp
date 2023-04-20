#include "syntax/abstractSyntaxTree.h"
#include "error/syntaxError.h"
#include "syntax/operation.h"

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

        struct OperatorInfo {
            Operation operation;
            int operandCount;
        };

        void popOperatorIntoOperand(std::vector<OperatorInfo>& operators, std::vector<SyntaxNode*>& operands) {
            OperatorInfo operatorInfo = operators.back();
            operators.pop_back();
            if (operands.size() < operatorInfo.operandCount) {
                throw SyntaxError("too few operands for operation (expected " + std::to_string(operatorInfo.operandCount) + ", got " + std::to_string(operands.size()) + ")");
            }
            if (operatorInfo.operation == Operation::Call) {
                SyntaxNode* callee = operands.at(operands.size() - operatorInfo.operandCount);
                std::vector<SyntaxNode*> arguments(operands.end() - operatorInfo.operandCount + 1, operands.end());
                operands.erase(operands.end() - operatorInfo.operandCount, operands.end());
                operands.push_back(new SyntaxNodeFunctionCall(callee, arguments));
            }
            else if (operatorInfo.operandCount == 1) {
                SyntaxNode* operand = operands.back();
                operands.back() = new SyntaxNodeUnaryExpression(operatorInfo.operation, operand);
            }
            else {
                SyntaxNode* rightSide = operands.back();
                operands.pop_back();
                SyntaxNode* leftSide = operands.back();
                operands.back() = new SyntaxNodeBinaryExpression(operatorInfo.operation, leftSide, rightSide);
            }
        }

        SyntaxNode* parseExpression(TokenIterator& iter, const TokenIterator& end, StopperRule stopper) {
            std::vector<OperatorInfo> operators;
            std::vector<SyntaxNode*> operands;
            bool expectOperand = true;
            while (iter != end && shouldUseToken(iter, end, stopper)) {
                if (std::holds_alternative<Symbol>(iter->value())) {
                    const Symbol& symbol = std::get<Symbol>(iter->value());
                    if (expectOperand && symbol == Symbol::ParenLeft) {
                        nextToken(iter, end);
                        operands.push_back(parseExpression(iter, end, StopperRule::ParenRight));
                        expectOperand = false;
                        ++iter;
                        continue;
                    }
                    Operation operation = findOperation(symbol, expectOperand);
                    if (operation == Operation::Unknown) {
                        throw SyntaxError("unexpected symbol '" + symbolNames.at(symbol) + "'", iter->start(), iter->end());
                    }
                    while (!operators.empty() && leftPrecedes(operators.back().operation, operation)) {
                        popOperatorIntoOperand(operators, operands);
                    }
                    OperatorInfo operatorInfo{operation};
                    switch (operation) {
                        case Operation::Call: {
                            operatorInfo.operandCount = 1;
                            nextToken(iter, end);
                            while (!iter->has(Symbol::ParenRight)) {
                                operands.push_back(parseExpression(iter, end, StopperRule::CommaOrParenRight));
                                ++operatorInfo.operandCount;
                                if (iter->has(Symbol::Comma)) {
                                    nextToken(iter, end);
                                }
                            }
                            break;
                        }
                        case Operation::PostInc:
                        case Operation::PostDec:
                        case Operation::ToNumber:
                        case Operation::Negate:
                        case Operation::PreInc:
                        case Operation::PreDec:
                        case Operation::Not:
                        case Operation::BitwiseNot:
                            operatorInfo.operandCount = 1;
                            break;
                        default:
                            operatorInfo.operandCount = 2;
                            break;
                    }
                    operators.push_back(operatorInfo);
                    expectOperand = findPrecedence(operatorInfo.operation) != Precedence::Postfix;
                }
                else {
                    if (!expectOperand) {
                        throw SyntaxError("expected an operator", iter->start(), iter->end());
                    }
                    else if (auto* value = std::get_if<IntValue>(&iter->value())) {
                        operands.push_back(new SyntaxNodeLiteral(*value));
                    }
                    else if (auto* value = std::get_if<FloatValue>(&iter->value())) {
                        operands.push_back(new SyntaxNodeLiteral(*value));
                    }
                    else if (auto* value = std::get_if<BoolValue>(&iter->value())) {
                        operands.push_back(new SyntaxNodeLiteral(*value));
                    }
                    else if (auto* value = std::get_if<StrValue>(&iter->value())) {
                        operands.push_back(new SyntaxNodeLiteral(*value));
                    }
                    else if (auto* value = std::get_if<Identifier>(&iter->value())) {
                        operands.push_back(new SyntaxNodeIdentifier(value->name));
                    }
                    else {
                        throw SyntaxError("unexpected token", iter->start(), iter->end());
                    }
                    expectOperand = false;
                }
                ++iter;
            }
            if (expectOperand) {
                --iter;
                throw SyntaxError("expected an operand", iter->end(), iter->end());
            }
            while (!operators.empty()) {
                popOperatorIntoOperand(operators, operands);
            }
            if (operands.size() != 1) {
                --iter;
                throw SyntaxError("expression unexpectedly resolved to " + std::to_string(operands.size()) + "operands", iter->end(), iter->end());
            }
            return operands.back();
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
                    auto* statement = block->statements.back();
                    block->statements.pop_back();
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
                if (iter != end && iter->has(Keyword::Else)) {
                    nextToken(iter, end);
                    auto* alternative = parseStatement(iter, end);
                    return new SyntaxNodeIfStatement(condition, consequent, alternative);
                } else {
                    --iter;
                    return new SyntaxNodeIfStatement(condition, consequent, nullptr);
                }
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
        out << "AbstractSyntaxTree{";
        for (int i = 0; i < tree.root().size(); ++i) {
            if (i != 0) {
                out << "; ";
            }
            out << tree.root()[i]->output();
        }
        return out << "}";
    }

}