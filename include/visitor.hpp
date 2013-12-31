#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "function_base.hpp"
#include "function_forward.hpp"

namespace ex {

/** \brief Common super-class for every visitor
 *
 *  There have to be listed overloaded \ref visit method for every function user
 *  as template parameter of \ref ex::function 
 *
 *  \todo Use Loki-style visitor generated from typelist.
 */
struct abstract_visitor {
    virtual void visit(function<base::variable> & c) = 0;
    virtual void visit(function<base::constant> & c) = 0;
    virtual void visit(function<base::sin> & c) = 0;
    virtual void visit(function<base::cos> & c) = 0;
    virtual void visit(function<base::tan> & c) = 0;
    virtual void visit(function<base::log> & c) = 0;
    virtual void visit(function<base::add> & c) = 0;
    virtual void visit(function<base::sub> & c) = 0;
    virtual void visit(function<base::mul> & c) = 0;
    virtual void visit(function<base::div> & c) = 0;
    virtual void visit(function<base::pow> & c) = 0;
};

/** \brief Real type of every visitor.
 *
 *  There have to be listed overloaded \ref visit method for every function used
 *  as template parameter of \ref ex::function
 *
 *  Thanks to this visitor don't have to have virtual visit functions. 
 *
 *  \tparam U visitor type
 *
 *  \todo Use Loki-style visitor generated from typelist.
 *         -- Huh, such a stupid idea. Shame yourself.
 */
template <typename U>
class generic_visitor : public abstract_visitor {
    /** \brief visitor instance */
    U u;
public:
    template <typename ... TList>
    generic_visitor(TList && ... plist) 
      : u(std::forward<TList>(plist)...) {
        u.pass_generic(this);
    }

    virtual void visit(function<base::variable> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::constant> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::sin> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::cos> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::tan> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::log> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::add> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::sub> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::mul> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::div> & c) {
        u.visit(c);
    }

    virtual void visit(function<base::pow> & c) {
        u.visit(c);
    }

    /** \brief Access visitor */
    U* operator-> () {
        return &u;
    }
    
};

}

#endif
