#ifndef EXPARSER_HPP
#define EXPARSER_HPP

#include <tuple>
#include <string>

#include "expression.hpp"

class exparser {
public:
    using return_type = std::tuple<bool, expression>;

    const bool debug = false;

private:
    static const return_type return_false;
    return_type loop(const std::string& s);

    return_type parse_parenthesis(const std::string& s);
    return_type parse_constant(const std::string& s);
    return_type parse_variable(const std::string& s);
    return_type parse_plus(const std::string& s);
    return_type parse_minus(const std::string& s);
    return_type parse_multiply(const std::string& s);
    return_type parse_divide(const std::string& s);
    return_type parse_power(const std::string& s);
    return_type parse_function(const std::string& s);
public:
    expression parse(const std::string& s);
};

#endif
