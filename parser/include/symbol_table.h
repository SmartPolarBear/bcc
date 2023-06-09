//
// Created by bear on 6/9/2023.
//

#pragma once

#include <memory>
#include <optional>
#include <string_view>

namespace parser
{

enum class symbol_type
{
    TYPENAME, FUNCTION, VARIABLE, STRUCT, ENUM, UNION
};

class symbol
{

};

class symbol_table
{
public:
    std::optional<std::shared_ptr<symbol>> get(std::string_view name);

    bool contains(std::string_view name);

    void put(std::shared_ptr<symbol> sym);

private:

};
}