#include "visitor_impl.hpp"

#include <boost/lexical_cast.hpp>

#include "function.hpp"

namespace ex {

void printer_visitor::visit(ex::expr & e) {
    return_("general_expression");
}

void printer_visitor::visit(ex::function<ex::base::variable> & c) {
    return_(c.value);
}

void printer_visitor::visit(ex::function<ex::base::constant> & c) {
    return_(std::to_string(c.value));
}

void printer_visitor::visit(ex::function<ex::base::sin> & c) {
    return_( "sin(" + recurse(c, 0) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::cos> & c) {
    return_( "cos(" + recurse(c, 0) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::tan> & c) {
    return_( "tan(" + recurse(c, 0) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::log> & c) {
    return_( "log(" + recurse(c, 0) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::add> & c) {
    return_( "(" + recurse(c, 0) + "+" + recurse(c, 1) + "_" );
}

void printer_visitor::visit(ex::function<ex::base::sub> & c) {
    return_( "(" + recurse(c, 0) + "-" + recurse(c, 1) + "_" );
}

void printer_visitor::visit(ex::function<ex::base::mul> & c) {
    return_( "(" + recurse(c, 0) + "*" + recurse(c, 1) + "_" );
}

void printer_visitor::visit(ex::function<ex::base::div> & c) {
    return_( "(" + recurse(c, 0) + "/" + recurse(c, 1) + "_" );
}

void printer_visitor::visit(ex::function<ex::base::pow> & c) {
    return_( "(" + recurse(c, 0) + "/" + recurse(c, 1) + "_" );
}

eval_visitor::eval_visitor(const globals::valuation_type & v_)
  : v(v_) { }

eval_visitor::eval_visitor(const eval_visitor & e)
  : v(e.v) { }

void eval_visitor::visit(expr & e) {
    return_( 0 );
}

void eval_visitor::visit(function<base::variable> & c) {
    return_( v(c.value) );
}

void eval_visitor::visit(function<base::constant> & c) {
    return_( c.value );
}

void eval_visitor::visit(function<base::sin> & c) {
    return_( std::sin(recurse(c, 0)) );
}

void eval_visitor::visit(function<base::cos> & c) {
    return_( std::cos(recurse(c, 0)) );
}

void eval_visitor::visit(function<base::tan> & c) {
    return_( std::tan(recurse(c, 0)) );
}

void eval_visitor::visit(function<base::log> & c) {
    return_( std::log(recurse(c, 0)) );
}

void eval_visitor::visit(function<base::add> & c) {
    return_( recurse(c, 0) + recurse(c, 1) );
}

void eval_visitor::visit(function<base::sub> & c) {
    return_( recurse(c, 0) - recurse(c, 1) );
}

void eval_visitor::visit(function<base::mul> & c) {
    return_( recurse(c, 0) * recurse(c, 1) );
}

void eval_visitor::visit(function<base::div> & c) {
    return_( recurse(c, 0) / recurse(c, 1) );
}

void eval_visitor::visit(function<base::pow> & c) {
    return_( std::pow(recurse(c, 0), recurse(c, 1)) );
}

void is_constant_visitor::visit(ex::expr & e) {
    for (std::size_t i = 0; i < e.arity(); ++i)
        e[i]->accept(*gen_);
}

void is_constant_visitor::visit(function<base::variable> &) {
    result = false;
}

}


