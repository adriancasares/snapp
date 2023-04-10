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

}