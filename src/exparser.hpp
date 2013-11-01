#ifndef EXPARSER_HPP
#define EXPARSER_HPP

#include <tuple>
#include <string>

#include <boost/optional.hpp>

#include "expression.hpp"

/** \brief Parse given string and build syntax tree from it
 *
 *  \todo Remove dependency on boost::optional. It is proposed
 *        to replace it with std::optional.
 */
class exparser {
public:
    typedef boost::optional<expression> return_type;

    /** \brief Suppress debug messages during parsing? */
    static const bool debug = false;

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

    static bool isalnum(char c);
    static bool isalpha(char c);
public:
    expression parse(const std::string& s);
};

#endif
