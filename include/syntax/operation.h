#ifndef SYNTAX_OPERATION_H_
#define SYNTAX_OPERATION_H_

#include <iostream>
#include <string>
#include <map>

#include "symbol.h"

namespace Snapp {

    enum class Operation {
        Unknown,
        Access,
        PostInc,
        PostDec,
        Call,
        // Unary operations start here
        ToNumber,
        Negate,
        PreInc,
        PreDec,
        BitwiseNot,
        Not,
        // Binary operations start here
        Exponent,
        Multiply,
        Divide,
        Modulus,
        Add,
        Subtract,
        LessThan,
        GreaterThan,
        LessEqual,
        GreaterEqual,
        Equal,
        NotEqual,
        BitwiseAnd,
        BitwiseXor,
        BitwiseOr,
        And,
        Or,
        Assign,
    };

    const std::map<Operation, std::string> operationNames = {
        {Operation::Access, "Access"},
        {Operation::PostInc, "PostInc"},
        {Operation::PostDec, "PostDec"},
        {Operation::Call, "Call"},
        {Operation::ToNumber, "ToNumber"},
        {Operation::Negate, "Negate"},
        {Operation::PreInc, "PreInc"},
        {Operation::PreDec, "PreDec"},
        {Operation::BitwiseNot, "BitwiseNot"},
        {Operation::Not, "Not"},
        {Operation::Exponent, "Exponent"},
        {Operation::Multiply, "Multiply"},
        {Operation::Divide, "Divide"},
        {Operation::Modulus, "Modulus"},
        {Operation::Add, "Add"},
        {Operation::Subtract, "Subtract"},
        {Operation::LessThan, "LessThan"},
        {Operation::GreaterThan, "GreaterThan"},
        {Operation::LessEqual, "LessEqual"},
        {Operation::GreaterEqual, "GreaterEqual"},
        {Operation::Equal, "Equal"},
        {Operation::NotEqual, "NotEqual"},
        {Operation::BitwiseAnd, "BitwiseAnd"},
        {Operation::BitwiseXor, "BitwiseXor"},
        {Operation::BitwiseOr, "BitwiseOr"},
        {Operation::And, "And"},
        {Operation::Or, "Or"},
        {Operation::Assign, "Assign"},
    };

    const std::map<Symbol, Operation> symbolToOperation = {
        {Symbol::Dot, Operation::Access},
        {Symbol::Plus2, Operation::PostInc},
        {Symbol::Minus2, Operation::PostDec},
        {Symbol::ParenLeft, Operation::Call},
        {Symbol::Exponent, Operation::Exponent},
        {Symbol::Star, Operation::Multiply},
        {Symbol::Slash, Operation::Divide},
        {Symbol::Percent, Operation::Modulus},
        {Symbol::Plus, Operation::Add},
        {Symbol::Minus, Operation::Subtract},
        {Symbol::LessThan, Operation::LessThan},
        {Symbol::GreaterThan, Operation::GreaterThan},
        {Symbol::LessEqual, Operation::LessEqual},
        {Symbol::GreaterEqual, Operation::GreaterEqual},
        {Symbol::Equal2, Operation::Equal},
        {Symbol::NotEqual, Operation::NotEqual},
        {Symbol::Ampersand, Operation::BitwiseAnd},
        {Symbol::Caret, Operation::BitwiseXor},
        {Symbol::Pipe, Operation::BitwiseOr},
        {Symbol::Ampersand2, Operation::And},
        {Symbol::Pipe2, Operation::Or},
        {Symbol::Equal, Operation::Assign},
    };

    const std::map<Symbol, Operation> symbolToPrefixOperation = {
        {Symbol::Plus, Operation::ToNumber},
        {Symbol::Minus, Operation::Negate},
        {Symbol::Plus2, Operation::PreInc},
        {Symbol::Minus2, Operation::PreDec},
        {Symbol::Tilde, Operation::BitwiseNot},
        {Symbol::Exclamation, Operation::Not},
    };

    Operation findOperation(const Symbol& symbol, bool prefix = false);

    std::ostream& operator<<(std::ostream& out, const Operation& operation);

    enum class Precedence {
        Unknown,
        Assignment,
        LogicalOr,
        LogicalAnd,
        BitwiseOr,
        BitwiseXor,
        BitwiseAnd,
        Equality,
        Comparison,
        Additive,
        Multiplicative,
        Exponent,
        Prefix,
        Postfix,
        Access,
        Container,
    };

    const std::map<Precedence, std::string> precedenceNames = {
        {Precedence::Assignment, "Assignment"},
        {Precedence::LogicalOr, "LogicalOr"},
        {Precedence::LogicalAnd, "LogicalAnd"},
        {Precedence::BitwiseOr, "BitwiseOr"},
        {Precedence::BitwiseXor, "BitwiseXor"},
        {Precedence::BitwiseAnd, "BitwiseAnd"},
        {Precedence::Equality, "Equality"},
        {Precedence::Comparison, "Comparison"},
        {Precedence::Additive, "Additive"},
        {Precedence::Multiplicative, "Multiplicative"},
        {Precedence::Exponent, "Exponent"},
        {Precedence::Prefix, "Prefix"},
        {Precedence::Postfix, "Postfix"},
        {Precedence::Access, "Access"},
        {Precedence::Container, "Container"},
    };

    const std::map<Operation, Precedence> operationPrecedence = {
        {Operation::Access, Precedence::Access},
        {Operation::PostInc, Precedence::Postfix},
        {Operation::PostDec, Precedence::Postfix},
        {Operation::Call, Precedence::Postfix},
        {Operation::ToNumber, Precedence::Prefix},
        {Operation::Negate, Precedence::Prefix},
        {Operation::PreInc, Precedence::Prefix},
        {Operation::PreDec, Precedence::Prefix},
        {Operation::BitwiseNot, Precedence::Prefix},
        {Operation::Not, Precedence::Prefix},
        {Operation::Exponent, Precedence::Exponent},
        {Operation::Multiply, Precedence::Multiplicative},
        {Operation::Divide, Precedence::Multiplicative},
        {Operation::Modulus, Precedence::Multiplicative},
        {Operation::Add, Precedence::Additive},
        {Operation::Subtract, Precedence::Additive},
        {Operation::LessThan, Precedence::Comparison},
        {Operation::GreaterThan, Precedence::Comparison},
        {Operation::LessEqual, Precedence::Comparison},
        {Operation::GreaterEqual, Precedence::Comparison},
        {Operation::Equal, Precedence::Equality},
        {Operation::NotEqual, Precedence::Equality},
        {Operation::BitwiseAnd, Precedence::BitwiseAnd},
        {Operation::BitwiseXor, Precedence::BitwiseXor},
        {Operation::BitwiseOr, Precedence::BitwiseOr},
        {Operation::And, Precedence::LogicalAnd},
        {Operation::Or, Precedence::LogicalOr},
        {Operation::Assign, Precedence::Assignment},
    };

    Precedence findPrecedence(const Operation& operation);

    std::ostream& operator<<(std::ostream& out, const Precedence& precedence);

    bool isLeftToRightAssociative(const Precedence& precedence);

    bool leftPrecedes(const Operation& leftSide, const Operation& rightSide);

}

#endif /* SYNTAX_OPERATION_H_ */
