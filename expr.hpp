#ifndef EXPR_HPP
#define EXPR_HPP

/*
 * BIG TODO : Check exception safety and add exception specifications
 */

#include <functional>

#include "traits.hpp"

class expression;

namespace ex {

template <class T, class Enable = void>
class function;

/**
 * Virtual super-class for expression tree
 *
 * All important types and functions for outside users are 
 * defined here.
 */
class expr {
public:

    /* It might be better to use policy class instead */
    //using ptr_type = std::unique_ptr<expr>;
    typedef expression ptr_type;
    typedef std::string string_type;
    typedef double eval_type;
    typedef std::function<eval_type(string_type)> valuation_type;
    typedef std::size_t size_type;

    virtual ptr_type derive(const string_type& var) = 0;
    virtual ptr_type clone() = 0;
    virtual string_type to_string() = 0;
    virtual eval_type eval(const valuation_type& values) = 0;
    
    virtual size_type arity() = 0;
    virtual ptr_type & operator[] (size_type n) = 0;
};


} // end namespace ex

#endif
