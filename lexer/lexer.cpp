#include <format>
#include <string>

#include "lexer/lexer.h"
#include "lexer/exception.h"
#include "lexer/token.h"

using namespace lexer;

using namespace std;

generator<lexer::token> lexer::scanner::scan()
{
	while (!is_end())
	{
		start_ = current_;
		co_yield scan_next();
	}
	co_return;
}

lexer::token lexer::scanner::scan_next()
{
	char c = advance();
	switch (c)
	{
	case '.':
		if (match(".."))
		{
			return make_token(token_type::ELLIPSIS);
		}
		else
		{
			return make_token(token_type::DOT);
		}
		break;
	case '>':
		if (match(">="))
		{
			return make_token(token_type::RIGHT_ASSIGN);
		}
		else if (match('>'))
		{
			return make_token(token_type::RIGHT_OP);
		}
		else if (match('='))
		{
			return make_token(token_type::GE_OP);
		}
		else
		{
			return make_token(token_type::RIGHT_ANGLE);
		}
		break;
	case '<':
		if (match("<="))
		{
			return make_token(token_type::LEFT_ASSIGN);
		}
		else if (match('<'))
		{
			return make_token(token_type::LEFT_OP);
		}
		else if (match('='))
		{
			return make_token(token_type::LE_OP);
		}
		else
		{
			return make_token(token_type::LEFT_ANGLE);
		}
		break;
	case '=':
		if (match('='))
		{
			return make_token(token_type::EQ_OP);
		}
		else
		{
			return make_token(token_type::EQUAL);
		}
		break;
	case '!':
		if (match('='))
		{
			return make_token(token_type::NE_OP);
		}
		else
		{
			return make_token(token_type::NOT);
		}
		break;
	case '+':
		if (match('+'))
		{
			return make_token(token_type::INC_OP);
		}
		else if (match('='))
		{
			return make_token(token_type::ADD_ASSIGN);
		}
		else
		{
			return make_token(token_type::PLUS);
		}
		break;
	case '-':
		if (match('-'))
		{
			return make_token(token_type::DEC_OP);
		}
		else if (match('>'))
		{
			return make_token(token_type::PTR_OP);
		}
		else if (match('='))
		{
			return make_token(token_type::SUB_ASSIGN);
		}
		else
		{
			return make_token(token_type::MINUS);
		}
		break;
	case '*':
		if (match('='))
		{
			return make_token(token_type::MUL_ASSIGN);
		}
		else
		{
			return make_token(token_type::STAR);
		}
		break;
	case '/':
		if (match('='))
		{
			return make_token(token_type::DIV_ASSIGN);
		}
		else
		{
			return make_token(token_type::SLASH);
		}
		break;
	case '%':
		if (match('='))
		{
			return make_token(token_type::MOD_ASSIGN);
		}
		else
		{
			return make_token(token_type::PERCENT);
		}
		break;
	case '&':
		if (match('&'))
		{
			return make_token(token_type::AND_OP);
		}
		else if (match('='))
		{
			return make_token(token_type::AND_ASSIGN);
		}
		else
		{
			return make_token(token_type::AND);
		}
		break;
	case '|':
		if (match('|'))
		{
			return make_token(token_type::OR_OP);
		}
		else if (match('='))
		{
			return make_token(token_type::OR_ASSIGN);
		}
		else
		{
			return make_token(token_type::OR);
		}
		break;
	case '^':
		if (match('='))
		{
			return make_token(token_type::XOR_ASSIGN);
		}
		else
		{
			return make_token(token_type::CARET);
		}
		break;
	case '~':
		return make_token(token_type::TILDE);
		break;
	case '?':
		return make_token(token_type::QUESTION_MARK);
		break;
	case ':':
		return make_token(token_type::COLON);
		break;
	case ';':
		return make_token(token_type::SEMICOLON);
		break;
	case ',':
		return make_token(token_type::COMMA);
		break;
	case '(':
		return make_token(token_type::LEFT_PAREN);
		break;
	case ')':
		return make_token(token_type::RIGHT_PAREN);
		break;
	case '[':
		return make_token(token_type::LEFT_BRACKET);
		break;
	case ']':
		return make_token(token_type::RIGHT_BRACKET);
		break;
	case '{':
		return make_token(token_type::LEFT_BRACE);
		break;
	case '\'':
	{
		auto char_lit = advance();
		if (!match('\''))
		{
			throw lexer_syntax_error("Expected '", {line_});
		}
		return token{whole_lexeme(), token_type::CONSTANT, char_lit, {line_}};
		break;
	}
	case '"':
		return scan_string();

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return scan_number();
		break;

	default:
		return scan_identifier_or_keyword();
	}
}

token lexer::scanner::make_token(lexer::token_type t)
{
	return token();
}

token scanner::scan_string()
{
	while (peek() != '"' && !is_end())
	{
		if (peek() == '\n')
		{
			line_++;
		}
		advance();
	}
	if (is_end())
	{
		throw lexer_syntax_error("Expected '\"'", {line_});
	}
	advance(); // eat ending "
	return token{whole_lexeme(), token_type::STRING_LITERAL, whole_lexeme(), {line_}};
}

token scanner::scan_identifier_or_keyword()
{
	while (is_letter(peek()) || is_digit(peek()))
	{
		advance();
	}

	auto lexeme = whole_lexeme();
	auto type = token_type::IDENTIFIER;
	if (is_keyword(lexeme))
	{
		type = keyword_types_.at(lexeme);
	}
	else if (is_typename(lexeme))
	{
		type = token_type::TYPE_NAME;
	}
	return token{lexeme, type, lexeme, {line_}};
}

token scanner::scan_number()
{
	bool floating = false;
	bool hex = false;
	bool oct = false;
	bool binary = false;
	while (is_digit(peek()) || peek() == 'x' || peek() == 'X' || peek() == 'b' || peek() == 'B')
	{
		char c = advance();
		if (c == 'x' || c == 'X' || c == 'b' || c == 'B')
		{
			if (hex || binary)
			{
				throw lexer_syntax_error(std::format("Invalid character '{}' found in number", c), {line_});
			}
			if (c == 'x' || c == 'X')
			{
				hex = true;
			}
			else
			{
				binary = true;
			}
		}
	}
	if (peek() == '.' && is_digit(peek(1)))
	{
		advance();
		while (is_digit(peek()))
		{
			advance();
		}
		floating = true;
	}

	if (peek() == 'e' || peek() == 'E')
	{
		advance();
		if (peek() == '+' || peek() == '-')
		{
			advance();
		}
		if (!is_digit(peek()))
		{
			throw lexer_syntax_error("Expected digit after exponent", {line_});
		}
		while (is_digit(peek()))
		{
			advance();
		}
		floating = true;
	}

	auto lexeme = whole_lexeme();
	if (lexeme.starts_with('0') && lexeme.size() > 1)
	{
		oct = true;
	}

	if (!(hex ^ oct ^ binary))
	{
		throw lexer_syntax_error("Invalid number format", {line_});
	}
	else if (floating && (hex || oct || binary))
	{
		throw lexer_syntax_error("Invalid number format", {line_});
	}

	if (floating)
	{
		return token{lexeme, token_type::CONSTANT, std::stold(string{lexeme}), {line_}};
	}
	else if (hex)
	{
		return token{lexeme, token_type::CONSTANT, std::stoll(string{lexeme}, nullptr, 16), {line_}};
	}
	else if (oct)
	{
		return token{lexeme, token_type::CONSTANT, std::stoll(string{lexeme}, nullptr, 8), {line_}};
	}
	else if (binary)
	{
		return token{lexeme, token_type::CONSTANT, std::stoll(string{lexeme}, nullptr, 2), {line_}};
	}
	else
	{
		return token{lexeme, token_type::CONSTANT, std::stoll(string{lexeme}, nullptr, 10), {line_}};
	}
}

bool scanner::is_typename(std::string_view lexeme)
{
	return false;
}
