#ifndef SYNTAX_KEYWORD_H_
#define SYNTAX_KEYWORD_H_

#include <iostream>
#include <string>
#include <map>

namespace Snapp {

    enum class Keyword {
        Unknown,
        If,
        Else,
        While,
        For,
        Return,
        Public,
        Private,
        Class,
        Observe,
        As,
    };

    const std::map<Keyword, std::string> keywordNames = {
        {Keyword::If, "If"},
        {Keyword::Else, "Else"},
        {Keyword::While, "While"},
        {Keyword::For, "For"},
        {Keyword::Return, "Return"},
        {Keyword::Public, "Public"},
        {Keyword::Private, "Private"},
        {Keyword::Class, "Class"},
        {Keyword::Observe, "Observe"},
        {Keyword::As, "As"},
    };

    const std::map<std::string, Keyword> literalToKeyword = {
        {"if", Keyword::If},
        {"else", Keyword::Else},
        {"while", Keyword::While},
        {"for", Keyword::For},
        {"return", Keyword::Return},
        {"public", Keyword::Public},
        {"private", Keyword::Private},
        {"class", Keyword::Class},
        {"observe", Keyword::Observe},
        {"as", Keyword::As},
    };

    Keyword findKeyword(const std::string& content);

    std::ostream& operator<<(std::ostream& out, const Keyword& keyword);

}

#endif /* SYNTAX_KEYWORD_H_ */
