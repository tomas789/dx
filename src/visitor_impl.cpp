#include "visitor_impl.hpp"

#include <functional>

#include "function.hpp"

namespace ex {

void printer_visitor::visit(ex::expr & e) {
    return_("general_expression");
}

void printer_visitor::visit(ex::function<ex::base::variable> & c) {
    return_("x_" + std::to_string(c.value));
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
    return_( "(" + recurse(c, 0) + "+" + recurse(c, 1) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::sub> & c) {
    return_( "(" + recurse(c, 0) + "-" + recurse(c, 1) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::mul> & c) {
    return_( "(" + recurse(c, 0) + "*" + recurse(c, 1) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::div> & c) {
    return_( "(" + recurse(c, 0) + "/" + recurse(c, 1) + ")" );
}

void printer_visitor::visit(ex::function<ex::base::pow> & c) {
    return_( "(" + recurse(c, 0) + "/" + recurse(c, 1) + ")" );
}

void latex_visitor::visit(ex::expr & e) {
    return_("general_expression");
}

void latex_visitor::visit(ex::function<ex::base::variable> & c) {
    return_("x_" + std::to_string(c.value));
}

void latex_visitor::visit(ex::function<ex::base::constant> & c) {
    return_(std::to_string(c.value));
}

void latex_visitor::visit(ex::function<ex::base::sin> & c) {
    return_( "\\sin \\left( " + recurse(c, 0) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::cos> & c) {
    return_( "\\cos \\left( " + recurse(c, 0) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::tan> & c) {
    return_( "\\tan \\left( " + recurse(c, 0) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::log> & c) {
    return_( "\\log \\left( " + recurse(c, 0) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::add> & c) {
    return_( "\\left( " + recurse(c, 0) + "+" + recurse(c, 1) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::sub> & c) {
    return_( "\\left( " + recurse(c, 0) + "-" + recurse(c, 1) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::mul> & c) {
    return_( "\\left( " + recurse(c, 0) + "*" + recurse(c, 1) + " \\right)" );
}

void latex_visitor::visit(ex::function<ex::base::div> & c) {
    return_( "\\frac{ " + recurse(c, 0) + "}{" + recurse(c, 1) + " }" );
}

void latex_visitor::visit(ex::function<ex::base::pow> & c) {
    return_( "\\left( " + recurse(c, 0) + "/" + recurse(c, 1) + " \\right)" );
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

void hash_visitor::visit(expr & e) {
    std::size_t h = typeid(e).hash_code();
    for (std::size_t i = 0; i < e.arity(); ++i)
        h ^= recurse(e, i);

    return_( h );
}

void hash_visitor::visit(ex::function<base::variable> & e) {
    std::hash<decltype(e.value)> h;
    return_( h(e.value) );
}

void hash_visitor::visit(ex::function<base::constant> & e) {
    std::hash<decltype(e.value)> h;
    return_( h(e.value) );
}

}


