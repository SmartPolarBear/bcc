//
// Created by bear on 3/28/2023.
//

#pragma once

#include <experimental/generator>

template<typename T, typename Alloc=std::allocator<char>>
using generator = std::experimental::generator<T, Alloc>;