#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <functional>

struct globals {
    typedef std::string string_type;
    typedef double eval_type;
    typedef std::size_t size_type;

    typedef std::function<eval_type(string_type)> valuation_type; 
};

#endif
