#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <vector>

#include "expr.hpp"

class expression {
    ex::tree_type tree;

public:
    typedef ex::expr::string_type string_type;
    typedef ex::expr::valuation_type valuation_type;

    expression();
    expression(const expression&);
    expression(expression&&);
    expression(ex::tree_type&&);
    expression(double c);

    expression& operator= (const expression&);
    expression& operator= (expression&&);

    bool empty() const;
    string_type to_string() const;
    expression derive(const string_type &) const;
    double evaluate(const std::vector<double> & v);

    expression operator+ (const expression&) const;
    expression operator- (const expression&) const;
    expression operator* (const expression&) const;
    expression operator/ (const expression&) const;

    /**
     * TODO : Remove operator^. Overload pow function instead.
     *        reason is operator priority
     */
    expression operator^ (const expression&) const;

    friend std::ostream& operator<< (std::ostream&, const expression&);
    friend expression sin(const expression& e);
    friend expression cos(const expression& e);
    friend expression tan(const expression& e);
    friend expression log(const expression& e);
};

std::ostream& operator<< (std::ostream&, const expression&);

expression make_constant(double c);
expression make_variable(std::size_t v);
expression sin(const expression& e);
expression cos(const expression& e);
expression tan(const expression& e);
expression log(const expression& e);

#endif
