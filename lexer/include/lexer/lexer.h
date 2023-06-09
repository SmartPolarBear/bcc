#pragma once

#include <unordered_map>
#include <memory>

#include "generator.h"
#include "token.h"
#include "symbol_table.h"

namespace lexer
{
class scanner final
{
public:
    explicit scanner(std::string_view source, std::shared_ptr<parser::symbol_table> tbl)
            : source_(source), sym_(std::move(tbl))
    {
        start_ = source_.begin();
        current_ = source_.begin();
    }

    [[nodiscard]] generator<token> scan();

private:
    // map C keywords to corresponding token_type
    const std::unordered_map<std::string_view, token_type> keyword_types_{
            {"auto",     token_type::AUTO},
            {"break",    token_type::BREAK},
            {"case",     token_type::CASE},
            {"char",     token_type::CHAR},
            {"const",    token_type::CONST},
            {"continue", token_type::CONTINUE},
            {"default",  token_type::DEFAULT},
            {"do",       token_type::DO},
            {"double",   token_type::DOUBLE},
            {"else",     token_type::ELSE},
            {"enum",     token_type::ENUM},
            {"extern",   token_type::EXTERN},
            {"float",    token_type::FLOAT},
            {"for",      token_type::FOR},
            {"goto",     token_type::GOTO},
            {"if",       token_type::IF},
            {"int",      token_type::INT},
            {"long",     token_type::LONG},
            {"register", token_type::REGISTER},
            {"return",   token_type::RETURN},
            {"short",    token_type::SHORT},
            {"signed",   token_type::SIGNED},
            {"sizeof",   token_type::SIZEOF},
            {"static",   token_type::STATIC},
            {"struct",   token_type::STRUCT},
            {"switch",   token_type::SWITCH},
            {"typedef",  token_type::TYPEDEF},
            {"union",    token_type::UNION},
            {"unsigned", token_type::UNSIGNED},
            {"void",     token_type::VOID},
            {"volatile", token_type::VOLATILE},
            {"while",    token_type::WHILE},
    };

    token scan_next();

    token scan_string();

    token scan_number();

    token scan_identifier_or_keyword();

    [[nodiscard]] bool is_keyword(std::string_view lexeme) const
    {
        return keyword_types_.contains(lexeme);
    }

    bool is_typename(std::string_view lexeme);

    bool is_end()
    {
        return current_ == source_.end();
    }

    char advance()
    {
        return *current_++;
    }

    char peek()
    {
        if (is_end())
        {
            return 0;
        }
        return *current_;
    }

    char peek(int n)
    {
        if (current_ + n >= source_.end())
        {
            return 0;
        }
        return *(current_ + n);
    }

    bool match(char expected)
    {
        if (is_end())
        {
            return false;
        }
        if (*current_ != expected)
        {
            return false;
        }
        current_++;
        return true;
    }

    bool match(std::string_view expected)
    {
        if (current_ + expected.size() > source_.end())
        {
            return false;
        }
        for (size_t i = 0; i < expected.size(); i++)
        {
            if (current_[i] != expected[i])
            {
                return false;
            }
        }
        current_ += expected.size();
        return true;
    }

    std::string_view whole_lexeme()
    {
        return {start_, current_};
    }

    static inline constexpr bool is_digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    static inline constexpr bool is_letter(char c)
    {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }

    token make_token(token_type t);

    std::string_view source_;
    std::string_view::iterator start_, current_;
    size_t line_{1}, col_{1};

    std::shared_ptr<parser::symbol_table> sym_;
};
}