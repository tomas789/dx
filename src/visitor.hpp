#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <boost/any.hpp>

#include "function_base.hpp"
#include "function_forward.hpp"

namespace ex {

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

template <typename U>
class generic_visitor : public abstract_visitor {
    U u;
public:
    generic_visitor() {
        u.pass_generic(this);
    }

    generic_visitor(const U & u_) : u(u_) {
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

    U* operator-> () {
        return &u;
    }
    
};

}

#endif
