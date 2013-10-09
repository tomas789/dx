#include <boost/any.hpp>

#include "expression.hpp"

#include "visitor.hpp"
#include "exparser.hpp"
#include "function_base.hpp"
#include "function.hpp"
#include "expr.hpp"
#include "n_ary.hpp"
#include "visitor.hpp"
#include "visitor_impl.hpp"

expression::expression()
  : tree() { }

expression::expression(const expression & expr) {
    *this = expr;
}

expression::expression(expression && expr) {
    *this = std::move(expr);
}

expression::expression(ex::expr * e) : tree(e) {
}

expression::expression(std::unique_ptr<ex::expr> && unique) 
    : tree(std::move(unique)) {
}

expression& expression::operator= (const expression & expr) {
    if (expr.empty()) return *this = expression();
    return *this = std::move(expr.tree->clone());
}

expression& expression::operator= (expression && expr) { 
    tree = std::move(expr.tree); 
    return *this; 
}

bool expression::empty() const {
    return !tree.get();
}

expression::string_type expression::to_string() const {
    if (!empty()) {
        ex::generic_visitor<ex::printer_visitor> v;
        auto res = tree->accept(v);
        return boost::any_cast<std::string>(res);
    } else return "";
}

expression expression::derive(const string_type& val) const {
    if (! empty()) 
        return expression(std::move(tree->derive(val)));
    return expression();
}

double expression::evaluate(const valuation_type & v) {
    ex::eval_visitor ev(v);
    ex::generic_visitor<ex::eval_visitor> visitor(ev);
    auto res = tree->accept(visitor);
    return boost::any_cast<double>(res);
}

std::unique_ptr<ex::expr> & expression::operator-> () {
    return tree;
}

const std::unique_ptr<ex::expr> & expression::operator-> () const {
    return tree;
}

expression expression::operator+ (const expression& expr) const {
    return expression(
        new ex::function<ex::base::add>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator- (const expression& expr) const {
    return expression(
        new ex::function<ex::base::sub>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator* (const expression& expr) const {
    return expression(
        new ex::function<ex::base::mul>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator/ (const expression& expr) const {
    return expression(
        new ex::function<ex::base::div>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression parse(const std::string & s) {
    exparser p;
    return p.parse(s);
}

std::ostream& operator<< (std::ostream& out, const expression& expr) {
    ex::generic_visitor<ex::printer_visitor> v;
    auto res = expr.tree->accept(v);
    return out << boost::any_cast<std::string>(res);
}

expression make_constant(double c) {
    return expression(new ex::function<ex::base::constant>(c));
}

expression make_variable(const std::string & var) {
    return expression(new ex::function<ex::base::variable>(var));
}

expression sin(const expression& e) {
    return expression(new ex::function<ex::base::sin>(std::move(e.tree->clone())));
}

expression cos(const expression& e) {
    return expression(new ex::function<ex::base::cos>(std::move(e.tree->clone())));
}

expression tan(const expression& e) {
    return expression(new ex::function<ex::base::tan>(std::move(e.tree->clone())));
}

expression log(const expression & e) {
    return expression(new ex::function<ex::base::log>(std::move(e.tree->clone())));
}

expression pow(const expression & base, const expression & exp) {
    return expression(new ex::function<ex::base::pow>(base.tree->clone(), exp.tree->clone()));
}

