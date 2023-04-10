#include "syntax/operation.h"

namespace Snapp {

    Operation findOperation(const Symbol& symbol, bool prefix) {
        const std::map<Symbol, Operation>& lookup = prefix ? symbolToPrefixOperation : symbolToOperation;
        auto it = lookup.find(symbol);
        return it == lookup.end() ? Operation::Unknown : it->second;
    }
  
    std::ostream& operator<<(std::ostream& out, const Operation& operation) {
        out << "Operation::";
        auto it = operationNames.find(operation);
        return it == operationNames.end() ? out << "Unknown" : out << it->second;
    }

    Precedence findPrecedence(const Operation& operation) {
        auto it = operationPrecedence.find(operation);
        return it == operationPrecedence.end() ? Precedence::Unknown : it->second;
    }

    std::ostream& operator<<(std::ostream& out, const Precedence& precedence) {
        out << "Precedence::";
        auto it = precedenceNames.find(precedence);
        return it == precedenceNames.end() ? out << "Unknown" : out << it->second;
    }

    bool isLeftToRightAssociative(const Precedence& precedence) {
        return !(precedence == Precedence::Prefix || precedence == Precedence::Assignment);
    }

    bool leftPrecedes(const Operation& leftSide, const Operation& rightSide) {
        Precedence leftPrecedence = findPrecedence(leftSide);
        Precedence rightPrecedence = findPrecedence(rightSide);
        return leftPrecedence > rightPrecedence || (leftPrecedence == rightPrecedence && isLeftToRightAssociative(leftPrecedence));
    }

}