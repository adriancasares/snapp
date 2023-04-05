#include "token.h"

namespace Snapp {

    std::optional<Symbol> findSymbol(std::string content) {
        auto it = literalToSymbol.find(content);
        return it == literalToSymbol.end() ? std::nullopt : std::optional<Symbol>(it->second);
    }
    
    std::ostream& operator<<(std::ostream& out, const Symbol& symbol) {
        out << "Symbol::";
        auto it = symbolNames.find(symbol);
        return it == symbolNames.end() ? out << "Unknown" : out << it->second;
    }

    std::optional<Keyword> findKeyword(std::string content) {
        auto it = literalToKeyword.find(content);
        return it == literalToKeyword.end() ? std::nullopt : std::optional<Keyword>(it->second);
    }
    
    std::ostream& operator<<(std::ostream& out, const Keyword& keyword) {
        out << "Keyword::";
        auto it = keywordNames.find(keyword);
        return it == keywordNames.end() ? out << "Unknown" : out << it->second;
    }

    SourceLocation::SourceLocation() {
        mIndex = 0;
        mLine = 1;
        mColumn = 1;
    }

    SourceLocation::SourceLocation(int index, int line, int column) {
        mIndex = index;
        mLine = line;
        mColumn = column;
    }

    int SourceLocation::index() const {
        return mIndex;
    }

    int SourceLocation::line() const {
        return mLine;
    }

    int SourceLocation::column() const {
        return mColumn;
    }

    std::ostream& operator<<(std::ostream& out, const SourceLocation& location) {
        return out << "(line " << location.line() << ":" << location.column() << ")";
    }

    const TokenValue& Token::value() const {
        return mValue;
    }

    const SourceLocation& Token::start() const {
        return mStart;
    }

    const SourceLocation& Token::end() const {
        return mEnd;
    }

}