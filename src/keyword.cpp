#include "keyword.h"

namespace Snapp {
  Keyword findKeyword(const std::string& content) {
    auto it = literalToKeyword.find(content);
    return it == literalToKeyword.end() ? Keyword::Unknown : it->second;
  }

  std::ostream& operator<<(std::ostream& out, const Keyword& keyword) {
    out << "Keyword::";
    auto it = keywordNames.find(keyword);
    return it == keywordNames.end() ? out << "Unknown" : out << it->second;
  }
}