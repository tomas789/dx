#include "visitor_impl.hpp"

#include <boost/lexical_cast.hpp>

#include "function.hpp"

namespace ex {

std::string printer_visitor::recursive(ex::expr & e, std::size_t num) {
    ex::generic_visitor<printer_visitor> v;
    return boost::any_cast<std::string>(
            e[num]->accept(v)
        );
}

boost::any printer_visitor::visit(ex::expr & e) {
    return std::string("general_expression");
}

boost::any printer_visitor::visit(ex::function<ex::base::variable> & c) {
    return c.value;
}

boost::any printer_visitor::visit(ex::function<ex::base::constant> & c) {
    return boost::lexical_cast<std::string>(c.value);
}

boost::any printer_visitor::visit(ex::function<ex::base::sin> & c) {
    return std::string("sin(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::cos> & c) {
    return std::string("cos(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::tan> & c) {
    return std::string("tan(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::log> & c) {
    return std::string("log(") + recursive(c, 0) + std::string(")");
}

boost::any printer_visitor::visit(ex::function<ex::base::add> & c) {
    return std::string("(" + recursive(c, 0) + "+" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::sub> & c) {
    return std::string("(" + recursive(c, 0) + "-" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::mul> & c) {
    return std::string("(" + recursive(c, 0) + "*" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::div> & c) {
    return std::string("(" + recursive(c, 0) + "/" + recursive(c, 1) + ")");
}

boost::any printer_visitor::visit(ex::function<ex::base::pow> & c) {
    return std::string("(" + recursive(c, 0) + "^" + recursive(c, 1) + ")");
}

eval_visitor::eval_visitor(const globals::valuation_type & v_)
  : v(v_) {
}

eval_visitor::eval_visitor(const eval_visitor & e)
  : v(e.v) {
}

globals::eval_type eval_visitor::recursive(expr & e, globals::size_type num) {
    eval_visitor ev(*this);
    generic_visitor<eval_visitor> v(ev);
    return boost::any_cast<globals::eval_type>(
            e[num]->accept(v)
        );
}

boost::any eval_visitor::visit(expr & e) {
    return 0;
}

boost::any eval_visitor::visit(function<base::variable> & c) {
    return v(c.value);
}

boost::any eval_visitor::visit(function<base::constant> & c) {
    return c.value;
}

boost::any eval_visitor::visit(function<base::sin> & c) {
    return std::sin(recursive(c, 0));
}

boost::any eval_visitor::visit(function<base::cos> & c) {
    return std::cos(recursive(c, 0));
}

boost::any eval_visitor::visit(function<base::tan> & c) {
    return std::tan(recursive(c, 0));
}

boost::any eval_visitor::visit(function<base::log> & c) {
    return std::log(recursive(c, 0));
}

boost::any eval_visitor::visit(function<base::add> & c) {
    return recursive(c, 0) + recursive(c, 1);
}

boost::any eval_visitor::visit(function<base::sub> & c) {
    return recursive(c, 0) - recursive(c, 1);
}

boost::any eval_visitor::visit(function<base::mul> & c) {
    return recursive(c, 0) * recursive(c, 1);
}

boost::any eval_visitor::visit(function<base::div> & c) {
    return recursive(c, 0) / recursive(c, 1);
}

boost::any eval_visitor::visit(function<base::pow> & c) {
    return std::pow(recursive(c, 0), recursive(c, 1));
}

bool is_constant_visitor::recursive(ex::expr & e, std::size_t num) {
    ex::generic_visitor<is_constant_visitor> v;
    return boost::any_cast<bool>(
            e[num]->accept(v)
        );
}

boost::any is_constant_visitor::visit(ex::expr & e) {
    for (std::size_t i = 0; i < e.arity(); ++i)
        if (!recursive(e, i)) return false;

    return true;
}

boost::any is_constant_visitor::visit(function<base::variable> &) {
    return false;
}

boost::any is_constant_visitor::visit(function<base::constant> &) {
    return true;
}

}


