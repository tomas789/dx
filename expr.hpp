#ifndef EXPR_HPP
#define EXPR_HPP

/*
 * BIG TODO : Check exception safety and add exception specifications
 */

#include <functional>

#include "globals.hpp"

class expression;

namespace ex {

struct abstract_visitor;

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

    virtual ptr_type clone() = 0;
    
    virtual size_type arity() = 0;
    virtual ptr_type & operator[] (size_type n) = 0;

    virtual void accept(abstract_visitor & v) = 0;
};


} // end namespace ex

#endif
