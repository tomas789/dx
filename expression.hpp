#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <memory>

#include "expr.hpp"

namespace ex {
    struct abstract_visitor;
}

class expression {
    std::unique_ptr<ex::expr> tree;

public:
    typedef std::size_t size_type;
    typedef std::string string_type;
    typedef double eval_type;
    typedef std::function<eval_type(string_type)> valuation_type;

    expression();
    expression(const expression &);
    expression(expression &&);
    explicit expression(ex::expr * e);
    expression(std::unique_ptr<ex::expr> &&);

    expression& operator= (const expression &);
    expression& operator= (expression &&);

    bool empty() const;
    size_type arity() const;
    expression & operator[] (size_type n);
    string_type to_string() const;
    expression derive(const string_type &) const;
    eval_type evaluate(const valuation_type & v);
    
    expression & simplify();
    bool is_constant();

    std::unique_ptr<ex::expr> & operator-> ();
    const std::unique_ptr<ex::expr> & operator-> () const;

    expression operator+ (const expression &) const;
    expression operator- (const expression &) const;
    expression operator* (const expression &) const;
    expression operator/ (const expression &) const;

    friend std::ostream& operator<< (std::ostream &, const expression &);
    friend expression sin(const expression & e);
    friend expression cos(const expression & e);
    friend expression tan(const expression & e);
    friend expression log(const expression & e);
    friend expression pow(const expression & base, const expression & exp);
};

expression parse(const std::string & s);

std::ostream& operator<< (std::ostream &, const expression &);

expression make_constant(double c);
expression make_variable(const std::string & v);
expression sin(const expression & e);
expression cos(const expression & e);
expression tan(const expression & e);
expression log(const expression & e);
expression pow(const expression & base, const expression & exp);

#endif
