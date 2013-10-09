#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <type_traits>
#include <cmath>

#include "function_forward.hpp"
#include "expression.hpp"
#include "n_ary.hpp"
#include "function_base.hpp"
#include "traits.hpp"
#include "expr.hpp"
#include "visitor.hpp"

namespace ex {

/**
 * Function class with all functionality
 * 
 * Template parameter must be derived from function_base class - this makes
 * more human-readable compiler errors when invalid template parameter is
 * supplied.
 *
 * TODO : Conditionally derive from n_ary only when T::arity != 0 (?)
 * TODO : Leverage generic design and make generic to_string and eval
 */
//template <class T, class Enable = void>
//class function;

template <class T>
class function<
    T, 
    typename std::enable_if<
        std::is_base_of<
            base::function_base, 
            T
        >::value 
    >::type 
>
  : public T, public n_ary<T::arity> {
public:
    /**
     * Constructor for functions with internal value
     * Defined only for functions with value_type type
     *
     * TODO : Is possible to simply pass more then one parameter
     *        without additional encapsulation (pair, tuple, vector, ...)?
     * TODO : define template <class T> function<T>::operator std::string();
     */
    
    /**
     * Parameterless constructor 
     */
    function() {
    }

    /**
     * Construct using value
     * Fro nullary functions only (T::arity == 0)
     */
    template <
            typename U = T, 
            typename std::enable_if<
                    has_value<U>::value
                >::type ...
        >
    function(const typename U::value_type& v) { 
        U::value = v; 
    }

    /**
     * Initialize using childrens
     * For non-terminals only (T::arity != 0)
     */
    template<
            typename... Tree, 
            typename = typename std::enable_if<
                    sizeof...(Tree) == T::arity && 
                    are_same<expr::ptr_type, Tree...>::value
                >::type
        >
    explicit function(Tree && ... tlist) {
        n_ary<T::arity>::move_to(std::move(tlist)...);
    }

    /** 
     * Copy constructor 
     */
    explicit function(const function<T>& a) : T(static_cast<T>(a)) { 
        for (std::size_t i = 0; i < T::arity; ++i)
            n_ary<T::arity>::childs[i] = std::move(a.childs[i]->clone());
    };

    /**
     * Implicit type cast to tree_type
     */
    operator expr::ptr_type() const { 
        return ptr_type(this); 
    }

    expr::ptr_type derive(const expr::string_type & var);
    expr::eval_type eval(const expr::valuation_type & values);

    /**
     * Virtual copy constructor using actual copy constructor
     */
    expr::ptr_type clone() {
        return expr::ptr_type(new function<T>(*this));
    }

    boost::any accept(abstract_visitor & v) {
        return v.visit(*this);
    }
};

/**
 * Construct terminal functions by it's value
 * TODO : NOT TESTED
 */
template <
        class T, 
        class = typename std::enable_if<
                std::is_base_of<
                        base::function_base, 
                        T
                    >::value 
            >::type 
    >
expr::ptr_type construct(typename T::value_type v) {
    return expr::ptr_type(nullptr);
}

/**
 * Required prototypes
 */
//template <> template <> 
//inline function<constant>::function(const constant::value_type& val);

//template <> template <>
//inline function<variable>::function(const variable::value_type& val);

/*******************************************************
 ***** function<variable> template specialisations *****
 *******************************************************/

//template <> template <> 
//inline function<variable>::function(const variable::value_type& vnum)
//{ variable::value = vnum; }

template <> 
inline expr::ptr_type function<base::variable>::derive(
        const expr::string_type & n) { 
    return make_constant(n == variable::value ? 1 : 0); 
}

template <> 
inline expr::eval_type function<base::variable>::eval(
        const expr::valuation_type & values) { 
    return values(variable::value); 
}

/*******************************************************
 ***** function<constant> template specialisations *****
 *******************************************************/

//template <> template <> 
//inline function<constant>::function(const constant::value_type& val)
//{ constant::value = val; }

template <> 
inline expr::ptr_type function<base::constant>::derive(
        const expr::string_type &) { 
    return make_constant(0); 
}

template <> 
inline expr::eval_type function<base::constant>::eval(
        const expr::valuation_type &) { 
    return value; 
}

/*******************************************************
 *****   function<sin> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::sin>::derive(
        const expr::string_type & var) {
    return cos(childs[0]) * childs[0].derive(var);
}

template <>
inline expr::eval_type function<base::sin>::eval(
        const expr::valuation_type & val) { 
    return std::sin(childs[0]->eval(val)); 
}

/*******************************************************
 *****   function<cos> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::cos>::derive(
        const expr::string_type & var) {
    return make_constant(-1) * sin(childs[0]) * childs[0]->derive(var);
}

template <>
inline expr::eval_type function<base::cos>::eval(
        const expr::valuation_type & val) { 
    return std::cos(childs[0]->eval(val)); 
}

/*******************************************************
 *****   function<tan> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::tan>::derive(
        const expr::string_type & var) {
    return childs[0].derive(var) / pow(cos(childs[0]), make_constant(2));
}

template <>
inline expr::eval_type function<base::tan>::eval(
        const expr::valuation_type & val) { 
    return std::tan(childs[0]->eval(val)); 
}

/*******************************************************
 *****   function<log> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::log>::derive(
        const expr::string_type & var) {
    return childs[0].derive(var) / childs[0];
}

template <>
inline expr::eval_type function<base::log>::eval(
        const expr::valuation_type & val) { 
    return std::log(childs[0]->eval(val)); 
}

/*******************************************************
 *****   function<add> template specialisations    *****
 *******************************************************/

template <> 
inline expr::ptr_type function<base::add>::derive(
        const expr::string_type & var) {
    return childs[0].derive(var) + childs[1].derive(var);
}

template <> 
inline expr::eval_type function<base::add>::eval(
        const expr::valuation_type & val) { 
    return childs[0]->eval(val) + childs[1]->eval(val); 
}

/*******************************************************
 *****   function<sub> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::sub>::derive(
        const expr::string_type & var) {
    return childs[0].derive(var) - childs[1].derive(var);    
}

template <>
inline expr::eval_type function<base::sub>::eval(
        const expr::valuation_type & val) { 
    return childs[0]->eval(val) - childs[1]->eval(val); 
}

/*******************************************************
 *****   function<mul> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::mul>::derive(
        const expr::string_type & var) {
    return childs[0].derive(var) * childs[1] + childs[0] * childs[1].derive(var);
}

template <>
inline expr::eval_type function<base::mul>::eval(
        const expr::valuation_type & val) { 
    return operator[](0)->eval(val) * operator[](1)->eval(val); 
}

/*******************************************************
 *****   function<div> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::div>::derive(
        const expr::string_type & var) {
    return (childs[0].derive(var) * childs[1] - childs[0] * childs[1].derive(var)) 
        / pow(childs[1], make_constant(2));
}

template <>
inline expr::eval_type function<base::div>::eval(
        const expr::valuation_type & val) { 
    return childs[0]->eval(val) / childs[1]->eval(val); 
}

/*******************************************************
 *****   function<pow> template specialisations    *****
 *******************************************************/

template <>
inline expr::ptr_type function<base::pow>::derive(
        const expr::string_type & var) {
    return ::pow(childs[0], childs[1]) * (childs[1] * ::log(childs[0])).derive(var);
}

template <>
inline expr::eval_type function<base::pow>::eval(
        const expr::valuation_type & val) { 
    return std::pow(childs[0]->eval(val), childs[1]->eval(val)); 
}

}

#endif
