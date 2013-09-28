#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <typeinfo>
#include "expr.hpp"

namespace std {
    template<
            class T,
            class Allocator
        > class vector;
}

class expression {
	ex::tree_type tree;

public:
	using string_type = ex::expr::string_type;
	using valuation_type = ex::expr::valuation_type;

	expression();
	expression(const expression&);
	expression(expression&&);
	expression(ex::tree_type&&);
	expression(double c);

	expression& operator= (const expression&);
	expression& operator= (expression&&);

	bool empty() const;
	string_type to_string() const;
	expression derive(const valuation_type::size_type&) const;
    double evaluate(const std::vector<double>& v);

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