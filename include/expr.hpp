#ifndef EXPR_HPP
#define EXPR_HPP

/*
 * BIG TODO : Check exception safety and add exception specifications
 */

#include <functional>
#include <memory>

#include "globals.hpp"

class expression;

namespace ex {

struct abstract_visitor;

/** \brief Virtual super-class for expression tree
 *
 *  All important types and functions for outside users are 
 *  defined here.
 */
class expr {
public:

    /* It might be better to use policy class instead */
    //using ptr_type = std::unique_ptr<expr>;
    typedef std::unique_ptr<expr> ptr_type;
    typedef std::string string_type;
    typedef double eval_type;
    typedef std::size_t size_type;
    typedef std::function<eval_type(size_type)> valuation_type;

    /** \brief Virtual copy-constructor
     *
     *  Return deep copy of this tree
     */
    virtual ptr_type clone() = 0;
   
    /** \brief Arity of root node of this tree
     */
    virtual size_type arity() = 0;

    /** \brief nth child of root node of this tree
     */
    virtual ptr_type & operator[] (size_type n) = 0;

    /** \brief Accept visitor
     *
     *  This is accepting function for visitor pattern.
     */
    virtual void accept(abstract_visitor & v) = 0;
};


} // end namespace ex

#endif
