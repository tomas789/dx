#include <stdexcept>
#include <cctype>
#include <algorithm>

#include "exparser.hpp"

const exparser::return_type exparser::return_false = 
    exparser::return_type(false, expression());

exparser::return_type exparser::loop(const std::string& s) {
    if (s.empty()) return return_false;
    
    if (debug) std::cout << "PARSING : " << s << std::endl;

    if (debug) std::cerr << "Try parenthesis" << std::endl;
    return_type parenthesis = parse_parenthesis(s);
    if (std::get<0>(parenthesis)) return parenthesis;
    
    if (debug) std::cerr << "Try constant" << std::endl;
    return_type constant = parse_constant(s);
    if (std::get<0>(constant)) return constant;
    
    if (debug) std::cerr << "Try variable" << std::endl;
    return_type variable = parse_variable(s);
    if (std::get<0>(variable)) return variable;
    
    if (debug) std::cerr << "Try plus" << std::endl;
    return_type plus = parse_plus(s);
    if (std::get<0>(plus)) return plus;
    
    if (debug) std::cerr << "Try minus" << std::endl;
    return_type minus = parse_minus(s);
    if (std::get<0>(minus)) return minus;

    if (debug) std::cerr << "Try multiply" << std::endl;
    return_type multiply = parse_multiply(s);
    if (std::get<0>(multiply)) return multiply;
    
    if (debug) std::cerr << "Try divide" << std::endl;
    return_type divide = parse_divide(s);
    if (std::get<0>(divide)) return divide;
    
    if (debug) std::cerr << "Try poewr" << std::endl;
    return_type power = parse_power(s);
    if (std::get<0>(power)) return power;
    
    if (debug) std::cerr << "Try function" << std::endl;
    return_type function = parse_function(s);
    if (std::get<0>(function)) return function;

    return return_false;
}

exparser::return_type exparser::parse_parenthesis(const std::string& s) {
    if ('(' != s[0] || ')' != s[s.length() - 1]) return return_false;
    return loop(s.substr(1, s.length() - 2));
}

exparser::return_type exparser::parse_constant(const std::string& s) {
    try {
        std::size_t pos = 0;
        auto c = stod(s, &pos);
        if (pos == s.length())
            return return_type(true, make_constant(c));
        else
            return return_false;
    } catch (std::invalid_argument e) {
        return return_false;
    } catch (std::out_of_range e) {
        throw e;
    }
}

exparser::return_type exparser::parse_variable(const std::string& s) {
    if (!isalpha(s[0])) return return_false;

    auto it = std::find_if_not(s.begin(), s.end(), isalnum);
    if (it != s.end()) return return_false;

    return return_type(true, make_variable(s));
}

exparser::return_type exparser::parse_plus(const std::string& s) {
    int par = 0;
    for (std::size_t i = s.length() - 1; i > 0; --i) {
        if ('(' == s[i]) ++par;
        else if (')' == s[i]) --par;
        else if (!par && '+' == s[i]) {
            auto lhs = loop(s.substr(0, i));
            auto rhs = loop(s.substr(i+1, std::string::npos));

            if (std::get<0>(lhs) && std::get<0>(rhs))
                return return_type(true, std::get<1>(lhs) + std::get<1>(rhs));
            else
                return return_false;
        }
    }

    return return_type(false, expression());
}

exparser::return_type exparser::parse_minus(const std::string& s) {
    int par = 0;
    for (std::size_t i = s.length() - 1; i > 0; --i) {
        if ('(' == s[i]) ++par;
        else if (')' == s[i]) --par;
        else if (!par && '-' == s[i]) {
            auto lhs = loop(s.substr(0, i));
            auto rhs = loop(s.substr(i+1, std::string::npos));

            if (std::get<0>(lhs) && std::get<0>(rhs))
                return return_type(true, std::get<1>(lhs) - std::get<1>(rhs));
            else
                return return_false;
        }
    }

    return return_type(false, expression());
}

exparser::return_type exparser::parse_multiply(const std::string& s) {
    int par = 0;
    for (std::size_t i = s.length() - 1; i > 0; --i) {
        if ('(' == s[i]) ++par;
        else if (')' == s[i]) --par;
        else if (!par && '*' == s[i]) {
            auto lhs = loop(s.substr(0, i));
            auto rhs = loop(s.substr(i+1, std::string::npos));

            if (std::get<0>(lhs) && std::get<0>(rhs))
                return return_type(true, std::get<1>(lhs) * std::get<1>(rhs));
            else
                return return_false;
        }
    }

    return return_type(false, expression());
}

exparser::return_type exparser::parse_divide(const std::string& s) {
    int par = 0;
    for (std::size_t i = s.length() - 1; i > 0; --i) {
        if ('(' == s[i]) ++par;
        else if (')' == s[i]) --par;
        else if (!par && '/' == s[i]) {
            auto lhs = loop(s.substr(0, i));
            auto rhs = loop(s.substr(i+1, std::string::npos));

            if (std::get<0>(lhs) && std::get<0>(rhs))
                return return_type(true, std::get<1>(lhs) / std::get<1>(rhs));
            else
                return return_false;
        }
    }

    return return_type(false, expression());
}

exparser::return_type exparser::parse_power(const std::string& s) {
    int par = 0;
    for (std::size_t i = 0; i < s.length(); ++i) {
        if ('(' == s[i]) ++par;
        else if (')' == s[i]) --par;
        else if (!par && '^' == s[i]) {
            auto lhs = loop(s.substr(0, i));
            auto rhs = loop(s.substr(i+1, std::string::npos));

            if (std::get<0>(lhs) && std::get<0>(rhs))
                return return_type(true, std::get<1>(lhs) ^ std::get<1>(rhs));
            else
                return return_false;
        }
    }

    return return_type(false, expression());
}

exparser::return_type exparser::parse_function(const std::string& s) {
    if (s[s.length() - 1] != ')') return return_false;
    std::size_t pos = s.find_first_of("(");
    if (pos == std::string::npos || !pos) return return_false;

    auto inner = loop(s.substr(pos, std::string::npos));
    if (!std::get<0>(inner)) return return_false;

    if (s.substr(0, pos) == "sin") 
        return return_type(true, sin(std::get<1>(inner)));
    else if (s.substr(0, pos) == "cos") 
        return return_type(true, cos(std::get<1>(inner)));
    else if (s.substr(0, pos) == "tan") 
        return return_type(true, tan(std::get<1>(inner)));
    else return return_false;
}

expression exparser::parse(const std::string& s) {
    return_type r = loop(s);
    if (std::get<0>(r)) return std::get<1>(r);
    else throw "unable to parse " + s;
}
