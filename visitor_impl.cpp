#include "visitor_impl.hpp"

#include <boost/lexical_cast.hpp>

#include "function.hpp"

namespace ex {

void printer_visitor::visit(ex::expr & e) {
    result += "general_expression";
}

void printer_visitor::visit(ex::function<ex::base::variable> & c) {
    result += c.value;
}

void printer_visitor::visit(ex::function<ex::base::constant> & c) {
    result += std::to_string(c.value);
}

void printer_visitor::visit(ex::function<ex::base::sin> & c) {
    result += "sin(";
    c[0]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::cos> & c) {
    result += "cos(";
    c[0]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::tan> & c) {
    result += "tan(";
    c[0]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::log> & c) {
    result = "log(";
    c[0]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::add> & c) {
    result += "(";
    c[0]->accept(*gen_);
    result += "+";
    c[1]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::sub> & c) {
    result += "(";
    c[0]->accept(*gen_);
    result += "-";
    c[1]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::mul> & c) {
    result += "(";
    c[0]->accept(*gen_);
    result += "*";
    c[1]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::div> & c) {
    result += "(";
    c[0]->accept(*gen_);
    result += "/";
    c[1]->accept(*gen_);
    result += ")";
}

void printer_visitor::visit(ex::function<ex::base::pow> & c) {
    result += "(";
    c[0]->accept(*gen_);
    result += "^";
    c[1]->accept(*gen_);
    result += ")";
}

eval_visitor::eval_visitor(const globals::valuation_type & v_)
  : v(v_) { }

eval_visitor::eval_visitor(const eval_visitor & e)
  : v(e.v) { }

void eval_visitor::visit(expr & e) {
    result = 0;
}

void eval_visitor::visit(function<base::variable> & c) {
    result = v(c.value);
}

void eval_visitor::visit(function<base::constant> & c) {
    result = c.value;
}

void eval_visitor::visit(function<base::sin> & c) {
    c[0]->accept(*gen_);
    result = std::sin(result);
}

void eval_visitor::visit(function<base::cos> & c) {
    c[0]->accept(*gen_);
    result = std::cos(result);
}

void eval_visitor::visit(function<base::tan> & c) {
    c[0]->accept(*gen_);
    result = std::tan(result);
}

void eval_visitor::visit(function<base::log> & c) {
    c[0]->accept(*gen_);
    result = std::log(result);
}

void eval_visitor::visit(function<base::add> & c) {
    c[0]->accept(*gen_);
    double lhs = result;
    c[1]->accept(*gen_);
    double rhs = result;
    result = lhs + rhs;
}

void eval_visitor::visit(function<base::sub> & c) {
    c[0]->accept(*gen_);
    double lhs = result;
    c[1]->accept(*gen_);
    double rhs = result;
    result = lhs - rhs;
}

void eval_visitor::visit(function<base::mul> & c) {
    c[0]->accept(*gen_);
    double lhs = result;
    c[1]->accept(*gen_);
    double rhs = result;
    result = lhs * rhs;
}

void eval_visitor::visit(function<base::div> & c) {
    c[0]->accept(*gen_);
    double lhs = result;
    c[1]->accept(*gen_);
    double rhs = result;
    result = lhs / rhs;
}

void eval_visitor::visit(function<base::pow> & c) {
    c[0]->accept(*gen_);
    double lhs = result;
    c[1]->accept(*gen_);
    double rhs = result;
    result = std::pow(lhs, rhs);
}

void is_constant_visitor::visit(ex::expr & e) {
    for (std::size_t i = 0; i < e.arity(); ++i)
        e[i]->accept(*gen_);
}

void is_constant_visitor::visit(function<base::variable> &) {
    result = false;
}

}


