//
// Created by bear on 3/30/2023.
//

#pragma once

#include <stdexcept>
#include <variant>

#include "lexer/location.h"
#include "lexer/token.h"
#include "lexer/location.h"

namespace lexer
{
class lexer_syntax_error : public std::runtime_error
{
public:
	lexer_syntax_error(const std::string &what_arg, token near)
		: where_(near), std::runtime_error(what_arg)
	{}

	lexer_syntax_error(const std::string &what_arg, source_location loc)
		: where_(loc), std::runtime_error(what_arg)
	{}

	[[nodiscard]] const std::variant<token, source_location> &where() const
	{
		return where_;
	}

	~lexer_syntax_error() override = default;

private:
	std::variant<token, source_location> where_;

};

}

