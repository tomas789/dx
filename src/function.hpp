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

/** \brief Function class with all functionality
 * 
 *  Template parameter must be derived from function_base class - this makes
 *  more human-readable compiler errors when invalid template parameter is
 *  supplied.
 *
 *  \tparam T class derived from \ref ex::base::function_base
 */
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
    /** \brief Default do-nothing constructor 
     *
     *  This should never be used (really?)
     */
    function() { }

    /** \brief Construct using value
     * 
     *  This method is defined for nullary functions 
     *  only (T::arity == 0)
     *
     *  \param v value of this node
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

    /** \brief Initialize using childrens
     * 
     *  Thid method is defined for non-terminals only (T::arity != 0)
     *
     *  Number of parameters have to by same as arity of this node.
     *  Trees will be used as childrend in the same order they was passed
     *  to this constructor.
     *
     *  \param tlist unique trees to be used as childs of this node
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

    /** \brief Copy constructor */
    explicit function(const function<T>& a) : T(static_cast<T>(a)) { 
        for (std::size_t i = 0; i < T::arity; ++i)
            n_ary<T::arity>::childs[i] = std::move(a.childs[i]->clone());
    };

    /** \brief Implicit type cast to \ref ex::expr::ptr_type */
    operator expr::ptr_type() const { 
        return ptr_type(this); 
    }

    /** \brief Virtual copy constructor implementation
     *
     *  This is using "real" copy-constructor to deep copy this tree
     */
    expr::ptr_type clone() {
        return expr::ptr_type(new function<T>(*this));
    }

    /** /brief Visitor pattern's acceptor implementation */
    void accept(abstract_visitor & v) {
        return v.visit(*this);
    }
};

}

#endif
