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

    /**
     * Virtual copy constructor using actual copy constructor
     */
    expr::ptr_type clone() {
        return expr::ptr_type(new function<T>(*this));
    }

    void accept(abstract_visitor & v) {
        return v.visit(*this);
    }
};

}

#endif
