#include "expression.hpp"

#include "expr.hpp"
#include "n_ary.hpp"

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
    if (! empty()) return tree->to_string();
    return string_type();
}

expression expression::derive(const string_type& val) const {
    if (! empty()) 
        return expression(std::move(tree->derive(val)));
    return expression();
}

double expression::evaluate(const valuation_type & v) {
    return tree->eval(v);
}

std::unique_ptr<ex::expr> & expression::operator-> () {
    return tree;
}

const std::unique_ptr<ex::expr> & expression::operator-> () const {
    return tree;
}

expression expression::operator+ (const expression& expr) const {
    return expression(
        new ex::function<ex::add>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator- (const expression& expr) const {
    return expression(
        new ex::function<ex::sub>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator* (const expression& expr) const {
    return expression(
        new ex::function<ex::mul>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator/ (const expression& expr) const {
    return expression(
        new ex::function<ex::div>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

expression expression::operator^ (const expression& expr) const {
    return expression(
        new ex::function<ex::pow>(
            std::move(tree->clone()), 
            std::move(expr.tree->clone())
        )
    );
}

std::ostream& operator<< (std::ostream& out, const expression& expr) {
    return out << expr.tree->to_string();
}

expression make_constant(double c) {
    return expression(new ex::function<ex::constant>(c));
}

expression make_variable(const std::string & var) {
    return expression(new ex::function<ex::variable>(var));
}

expression sin(const expression& e) {
    return expression(new ex::function<ex::sin>(std::move(e.tree->clone())));
}

expression cos(const expression& e) {
    return expression(new ex::function<ex::cos>(std::move(e.tree->clone())));
}

expression tan(const expression& e) {
    return expression(new ex::function<ex::tan>(std::move(e.tree->clone())));
}

