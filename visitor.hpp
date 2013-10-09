#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <boost/any.hpp>

#include "function_base.hpp"
#include "function_forward.hpp"

namespace ex {

struct abstract_visitor {
    virtual boost::any visit(function<base::variable> & c) = 0;
    virtual boost::any visit(function<base::constant> & c) = 0;
    virtual boost::any visit(function<base::sin> & c) = 0;
    virtual boost::any visit(function<base::cos> & c) = 0;
    virtual boost::any visit(function<base::tan> & c) = 0;
    virtual boost::any visit(function<base::log> & c) = 0;
    virtual boost::any visit(function<base::add> & c) = 0;
    virtual boost::any visit(function<base::sub> & c) = 0;
    virtual boost::any visit(function<base::mul> & c) = 0;
    virtual boost::any visit(function<base::div> & c) = 0;
    virtual boost::any visit(function<base::pow> & c) = 0;
};

template <typename U>
class generic_visitor : public abstract_visitor {
    U u;
    
public:
    generic_visitor() = default;
    generic_visitor(const U & u_) : u(u_) { }

    virtual boost::any visit(function<base::variable> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::constant> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::sin> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::cos> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::tan> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::log> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::add> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::sub> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::mul> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::div> & c) {
        return u.visit(c);
    }

    virtual boost::any visit(function<base::pow> & c) {
        return u.visit(c);
    }
    
};

}

#endif
