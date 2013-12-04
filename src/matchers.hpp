#ifndef _MATCHERS_HPP
#define _MATCHERS_HPP

#include "function_base.hpp"
#include "function.hpp"
#include "visitor.hpp"
#include "visitor_impl.hpp"

namespace ex {

bool are_same(expr * lhs, expr * rhs);

template <typename T>
bool is(expr * op) {
    return typeid(op) == typeid(function<T>);
}

}

#endif
