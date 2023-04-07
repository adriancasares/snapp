#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <variant>
#include <vector>

namespace Snapp {

    using Integer = int;
    using Float = double;
    using Bool = bool;
    using String = std::string;

    enum class Symbol {
        Unknown,
        Plus,
        Minus,
        Star,
        Slash,
        Percent,
        Exponent,
        Tilde,
        Ampersand,
        Caret,
        Pipe,
        Exclamation,
        Ampersand2,
        Caret2,
        Pipe2,
        LessThan2,
        GreaterThan2,
        Equal2,
        NotEqual,
        LessThan,
        GreaterThan,
        LessEqual,
        GreaterEqual,
        Equal,
        EvalEqual,
        Dot,
        Comma,
        Colon,
        Semicolon,
        ParenLeft,
        ParenRight,
        SquareLeft,
        SquareRight,
        CurlyLeft,
        CurlyRight,
        Question,
        AtSign,
        Hash,
        Dollar,
        Backslash,
        RangeExc,
        RangeInc,
        RightArrow,
        LambdaArrow,
    };
    
    const std::map<Symbol, std::string> symbolNames = {
        {Symbol::Plus, "Plus"},
        {Symbol::Minus, "Minus"},
        {Symbol::Star, "Star"},
        {Symbol::Slash, "Slash"},
        {Symbol::Percent, "Percent"},
        {Symbol::Exponent, "Exponent"},
        {Symbol::Tilde, "Tilde"},
        {Symbol::Ampersand, "Ampersand"},
        {Symbol::Caret, "Caret"},
        {Symbol::Pipe, "Pipe"},
        {Symbol::Exclamation, "Exclamation"},
        {Symbol::Ampersand2, "Ampersand2"},
        {Symbol::Caret2, "Caret2"},
        {Symbol::Pipe2, "Pipe2"},
        {Symbol::LessThan2, "LessThan2"},
        {Symbol::GreaterThan2, "GreaterThan2"},
        {Symbol::Equal2, "Equal2"},
        {Symbol::NotEqual, "NotEqual"},
        {Symbol::LessThan, "LessThan"},
        {Symbol::GreaterThan, "GreaterThan"},
        {Symbol::LessEqual, "LessEqual"},
        {Symbol::GreaterEqual, "GreaterEqual"},
        {Symbol::Equal, "Equal"},
        {Symbol::EvalEqual, "EvalEqual"},
        {Symbol::Dot, "Dot"},
        {Symbol::Comma, "Comma"},
        {Symbol::Colon, "Colon"},
        {Symbol::Semicolon, "Semicolon"},
        {Symbol::ParenLeft, "ParenLeft"},
        {Symbol::ParenRight, "ParenRight"},
        {Symbol::SquareLeft, "SquareLeft"},
        {Symbol::SquareRight, "SquareRight"},
        {Symbol::CurlyLeft, "CurlyLeft"},
        {Symbol::CurlyRight, "CurlyRight"},
        {Symbol::Question, "Question"},
        {Symbol::AtSign, "AtSign"},
        {Symbol::Hash, "Hash"},
        {Symbol::Dollar, "Dollar"},
        {Symbol::Backslash, "Backslash"},
        {Symbol::RangeExc, "RangeExc"},
        {Symbol::RangeInc, "RangeInc"},
        {Symbol::RightArrow, "RightArrow"},
        {Symbol::LambdaArrow, "LambdaArrow"},
    };
    
    const std::map<std::string, Symbol> literalToSymbol = {
        {"+", Symbol::Plus},
        {"-", Symbol::Minus},
        {"*", Symbol::Star},
        {"/", Symbol::Slash},
        {"%", Symbol::Percent},
        {"**", Symbol::Exponent},
        {"~", Symbol::Tilde},
        {"&", Symbol::Ampersand},
        {"^", Symbol::Caret},
        {"|", Symbol::Pipe},
        {"!", Symbol::Exclamation},
        {"&&", Symbol::Ampersand2},
        {"^^", Symbol::Caret2},
        {"||", Symbol::Pipe2},
        {"<<", Symbol::LessThan2},
        {">>", Symbol::GreaterThan2},
        {"==", Symbol::Equal2},
        {"!=", Symbol::NotEqual},
        {"<", Symbol::LessThan},
        {">", Symbol::GreaterThan},
        {"<=", Symbol::LessEqual},
        {">=", Symbol::GreaterEqual},
        {"=", Symbol::Equal},
        {":=", Symbol::EvalEqual},
        {".", Symbol::Dot},
        {",", Symbol::Comma},
        {":", Symbol::Colon},
        {";", Symbol::Semicolon},
        {"(", Symbol::ParenLeft},
        {")", Symbol::ParenRight},
        {"[", Symbol::SquareLeft},
        {"]", Symbol::SquareRight},
        {"{", Symbol::CurlyLeft},
        {"}", Symbol::CurlyRight},
        {"?", Symbol::Question},
        {"@", Symbol::AtSign},
        {"#", Symbol::Hash},
        {"$", Symbol::Dollar},
        {"\\", Symbol::Backslash},
        {"..", Symbol::RangeExc},
        {"..=", Symbol::RangeInc},
        {"->", Symbol::RightArrow},
        {"=>", Symbol::LambdaArrow},
    };
    
    Symbol findSymbol(const std::string& content);
    
    std::ostream& operator<<(std::ostream& out, const Symbol& symbol);
    
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

    struct Identifier {
        std::string name;
    };

    using TokenValue = std::variant<Symbol, Keyword, Identifier, Integer, Float, Bool, String>;

    struct SourceLocation {
        int index = 0;
        int line = 1;
        int column = 1;
    };

    std::ostream& operator<<(std::ostream& out, const SourceLocation& location);

    class Token {
      public:
          Token(TokenValue value, SourceLocation start, SourceLocation end);

          std::string output() const;
          const TokenValue& value() const;
          const SourceLocation& start() const;
          const SourceLocation& end() const;

      private:
          TokenValue value_;
          SourceLocation start_, end_;
    };

    class SyntaxError {
    public:
        SyntaxError();
        SyntaxError(std::string message);
        SyntaxError(std::string message, std::optional<SourceLocation> start, std::optional<SourceLocation> end);

        const std::string& message() const;
        const std::optional<SourceLocation>& start() const;
        const std::optional<SourceLocation>& end() const;

        std::string output(const std::string& sourceCode) const;

    private:
        std::string message_;
        std::optional<SourceLocation> start_, end_;
    };

    class Tokenizer {
    public:
        Tokenizer(const std::string& source);

        const SourceLocation& location() const;
        char nextChar();
        char peekChar();

        void pushToken(Token token);
        std::vector<Token> tokens() const;

    private:
        std::string::const_iterator chars_, end_;
        SourceLocation location_;
        std::vector<Token> tokens_;
    };

    std::vector<Token> getAllTokens(const std::string& source);

}

#endif