#include "symbol.h"

namespace Snapp {
  Symbol findSymbol(const std::string& content) {
    auto it = literalToSymbol.find(content);
    return it == literalToSymbol.end() ? Symbol::Unknown : it->second;
  }

  std::ostream& operator<<(std::ostream& out, const Symbol& symbol) {
    out << "Symbol::";
    auto it = symbolNames.find(symbol);
    return it == symbolNames.end() ? out << "Unknown" : out << it->second;
  }
}