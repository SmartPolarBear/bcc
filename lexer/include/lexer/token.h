//
// Created by bear on 3/28/2023.
//

#pragma once

#include <string_view>
#include <variant>
#include <cstdint>

#include "lexer/location.h"

namespace lexer
{
enum class token_type
{
// C keywords
	AUTO,
	BREAK,
	CASE,
	CHAR,
	CONST,
	CONTINUE,
	DEFAULT,
	DO,
	DOUBLE,
	ELSE,
	ENUM,
	EXTERN,
	FLOAT,
	FOR,
	GOTO,
	IF,
	INT,
	LONG,
	REGISTER,
	RETURN,
	SHORT,
	SIGNED,
	SIZEOF,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	VOLATILE,
	WHILE,

// C operators
	ELLIPSIS,
	RIGHT_ASSIGN,
	LEFT_ASSIGN,
	ADD_ASSIGN,
	SUB_ASSIGN,
	MUL_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	AND_ASSIGN,
	XOR_ASSIGN,
	OR_ASSIGN,
	RIGHT_OP,
	LEFT_OP,
	INC_OP,
	DEC_OP,
	PTR_OP,
	AND_OP,
	OR_OP,
	LE_OP,
	GE_OP,
	EQ_OP,
	NE_OP,

// C delimiters
	SEMICOLON,
	LEFT_BRACE,
	RIGHT_BRACE,
	COMMA,
	COLON,
	EQUAL,
	LEFT_PAREN,
	RIGHT_PAREN,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	DOT,
	AND,
	STAR,
	PLUS,
	MINUS,
	TILDE,
	NOT,
	SLASH,
	PERCENT,
	LEFT_ANGLE,
	RIGHT_ANGLE,
	CARET,
	OR,
	QUESTION_MARK,

// C literals
	IDENTIFIER,
	CONSTANT,
	TYPE_NAME,
	STRING_LITERAL,
};

struct token final
{
	std::string_view lexeme;
	token_type type;
	std::variant<std::string_view, char, int64_t, uint64_t, long double> value;
	source_location where;
};

}