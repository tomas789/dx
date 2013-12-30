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

/** \brief No purpose yet */
class visitor_common { };

/** \brief Common base for every visitor supplying base functionality for them
 *
 *  Deriving from this is optional but recommanded especially for recursive
 *  visitors.
 *
 *  \tparam Visitor visitor type
 *  \tparam Result return type of visitor
 *
 *  \todo Result should be defaulted to void. (Is it good idea?)
 */
template <typename Visitor, typename Result>
class visitor_base : public visitor_common {
protected:
    Result result;
    generic_visitor<Visitor> * gen_;

public:
    void pass_generic(generic_visitor<Visitor> * v) {
        gen_ = v;
    }

    /** \brief Obtain value generated from visitor
     *
     *  If called before visitor did return_( ... ) default value for
     *  result type is returned.
     */
    Result get_result() const {
        return result;
    }

    /** \brief Call visitor to nth child of node e */
    Result recurse(ex::expr & e, std::size_t n) {
        Result tmp;
        std::swap(result, tmp);
        e[n]->accept(*gen_);
        std::swap(result, tmp);
        return tmp;
    }

    /** \brief Return value from visitor */
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

class latex_visitor : public visitor_base<latex_visitor, std::string> {
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

/** \brief Evaluate tree for given valuation */
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

/** \brief UNUSED Semantically incorrect visitor
 *
 *  It should be removed. It is left here to remember not to 
 *  repeat the same mistake. You cannot linearize tree by 
 *  visiting it because you cannot guarantee it returns array
 *  of !unique! references to nodes.
 */
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

/** \brief True iff no leave is variable not matter the name is */
class is_constant_visitor : public visitor_base<is_constant_visitor, bool> {
public:
    void visit(ex::expr & e);
    void visit(function<base::variable> &);
};

/** \brief Generate hash for given tree */
class hash_visitor : public visitor_base<hash_visitor, std::size_t> {
public:
    void visit(expr & e);
    void visit(ex::function<base::variable> & e);
    void visit(ex::function<base::constant> & e);

};

}

#endif
