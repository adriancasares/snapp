//
// Created by Adrian Casares on 4/7/23.
//

#ifndef SYNTAX_KEYWORD_H_
#define SYNTAX_KEYWORD_H_

#include <iostream>
#include <string>
#include <map>

namespace Snapp {

    enum class Keyword {
        Unknown,
        As,
        If,
        Elif,
        Else,
        For,
        While,
        Public,
        Private,
        Class,
        Observe,
    };

    const std::map<Keyword, std::string> keywordNames = {
        {Keyword::As, "As"},
        {Keyword::If, "If"},
        {Keyword::Elif, "Elif"},
        {Keyword::Else, "Else"},
        {Keyword::For, "For"},
        {Keyword::While, "While"},
        {Keyword::Public, "Public"},
        {Keyword::Private, "Private"},
        {Keyword::Class, "Class"},
        {Keyword::Observe, "Observe"},
    };

    const std::map<std::string, Keyword> literalToKeyword = {
        {"as", Keyword::As},
        {"if", Keyword::If},
        {"elif", Keyword::Elif},
        {"else", Keyword::Else},
        {"for", Keyword::For},
        {"while", Keyword::While},
        {"public", Keyword::Public},
        {"private", Keyword::Private},
        {"class", Keyword::Class},
        {"observe", Keyword::Observe},
    };

    Keyword findKeyword(const std::string& content);

    std::ostream& operator<<(std::ostream& out, const Keyword& keyword);

}

#endif /* SYNTAX_KEYWORD_H_ */
