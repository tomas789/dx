#ifndef VISITOR_IMPL_HPP
#define VISITOR_IMPL_HPP

#include <string>

#include "expr.hpp"
#include "globals.hpp"
#include "visitor.hpp"
#include "function_base.hpp"
#include "function_forward.hpp"

namespace ex {

class printer_visitor {
    std::string recursive(ex::expr & e, std::size_t num);

public:
    boost::any visit(ex::expr & e);
    boost::any visit(ex::function<ex::base::variable> & c);
    boost::any visit(ex::function<ex::base::constant> & c);
    boost::any visit(ex::function<ex::base::sin> & c);
    boost::any visit(ex::function<ex::base::cos> & c);
    boost::any visit(ex::function<ex::base::tan> & c);
    boost::any visit(ex::function<ex::base::log> & c);
    boost::any visit(ex::function<ex::base::add> & c);
    boost::any visit(ex::function<ex::base::sub> & c);
    boost::any visit(ex::function<ex::base::mul> & c);
    boost::any visit(ex::function<ex::base::div> & c);
    boost::any visit(ex::function<ex::base::pow> & c);
};

class eval_visitor {
    const globals::valuation_type & v;
    globals::eval_type recursive(ex::expr & e, std::size_t num);

public:
    eval_visitor(const globals::valuation_type &);    
    eval_visitor(const eval_visitor &);

    boost::any visit(ex::expr & e);
    boost::any visit(ex::function<ex::base::variable> & c);
    boost::any visit(ex::function<ex::base::constant> & c);
    boost::any visit(ex::function<ex::base::sin> & c);
    boost::any visit(ex::function<ex::base::cos> & c);
    boost::any visit(ex::function<ex::base::tan> & c);
    boost::any visit(ex::function<ex::base::log> & c);
    boost::any visit(ex::function<ex::base::add> & c);
    boost::any visit(ex::function<ex::base::sub> & c);
    boost::any visit(ex::function<ex::base::mul> & c);
    boost::any visit(ex::function<ex::base::div> & c);
    boost::any visit(ex::function<ex::base::pow> & c);
};

class is_constant_visitor {
    bool recursive(expr & e, std::size_t num);
public:
    boost::any visit(ex::expr & e);
    boost::any visit(function<base::variable> &);
    boost::any visit(function<base::constant> &);
};

}

#endif
