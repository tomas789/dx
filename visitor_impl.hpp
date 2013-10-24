#ifndef VISITOR_IMPL_HPP
#define VISITOR_IMPL_HPP

#include <string>
#include <memory>
#include <vector>

#include "expr.hpp"
#include "expression.hpp"
#include "globals.hpp"
#include "visitor.hpp"
#include "function_base.hpp"
#include "function_forward.hpp"

namespace ex {

template <typename Visitor, typename Result>
class visitor_base {
protected:
    Result result;
    generic_visitor<Visitor> * gen_;

public:
    void pass_generic(generic_visitor<Visitor> * v) {
        gen_ = v;
    }

    Result get_result() const {
        return result;
    }

    Result recurse(ex::expr & e, std::size_t n) {
        Result tmp;
        std::swap(result, tmp);
        e[n]->accept(*gen_);
        std::swap(result, tmp);
        return tmp;
    }

    void return_(Result r) {
        result = r;
    }
};

class printer_visitor : public visitor_base<printer_visitor, std::string> {
public:
    void visit(ex::expr & e);
    void visit(ex::function<ex::base::variable> & c);
    void visit(ex::function<ex::base::constant> & c);
    void visit(ex::function<ex::base::sin> & c);
    void visit(ex::function<ex::base::cos> & c);
    void visit(ex::function<ex::base::tan> & c);
    void visit(ex::function<ex::base::log> & c);
    void visit(ex::function<ex::base::add> & c);
    void visit(ex::function<ex::base::sub> & c);
    void visit(ex::function<ex::base::mul> & c);
    void visit(ex::function<ex::base::div> & c);
    void visit(ex::function<ex::base::pow> & c);
};

class eval_visitor : public visitor_base<eval_visitor, double> {
    const globals::valuation_type & v;
public:
    eval_visitor(const globals::valuation_type &);
    eval_visitor(const eval_visitor &);

    void visit(ex::expr & e);
    void visit(ex::function<ex::base::variable> & c);
    void visit(ex::function<ex::base::constant> & c);
    void visit(ex::function<ex::base::sin> & c);
    void visit(ex::function<ex::base::cos> & c);
    void visit(ex::function<ex::base::tan> & c);
    void visit(ex::function<ex::base::log> & c);
    void visit(ex::function<ex::base::add> & c);
    void visit(ex::function<ex::base::sub> & c);
    void visit(ex::function<ex::base::mul> & c);
    void visit(ex::function<ex::base::div> & c);
    void visit(ex::function<ex::base::pow> & c);
};

class flatten_visitor : public visitor_base<
        flatten_visitor, 
        std::vector<std::unique_ptr<expr> *>
    > {
    
public:
    void visit(ex::expr & e);
    void visit(ex::function<ex::base::variable> & c);
    void visit(ex::function<ex::base::constant> & c);
    void visit(ex::function<ex::base::sin> & c);
    void visit(ex::function<ex::base::cos> & c);
    void visit(ex::function<ex::base::tan> & c);
    void visit(ex::function<ex::base::log> & c);
    void visit(ex::function<ex::base::add> & c);
    void visit(ex::function<ex::base::sub> & c);
    void visit(ex::function<ex::base::mul> & c);
    void visit(ex::function<ex::base::div> & c);
    void visit(ex::function<ex::base::pow> & c);
};

class is_constant_visitor : public visitor_base<is_constant_visitor, bool> {
public:
    void visit(ex::expr & e);
    void visit(function<base::variable> &);
};

}

#endif
