#ifndef N_ARY_HPP
#define N_ARY_HPP

#include <array>

#include "expr.hpp"
#include "globals.hpp"
#include "traits.hpp"

namespace ex {

/** \briefChild (subtree) container.
 *
 *  \tparam Arity Arity of this node
 */
template <std::size_t Arity>
class n_ary : public expr {
protected:
    /** \property childs
     *
     *  Array of child nodes. Each subtree is unique and garanteed not
     *  to be pointed to by another node.
     */
    std::array<expr::ptr_type, Arity> childs;

    template <class T, class U> friend class function;
public:
    /** \brief Get arity of this node */
    globals::size_type arity() {
        return Arity;
    }

    /** \brief Access child of this node
     *
     *  \param n zero based index of requested child node
     *  \return reference to unique pointer to requested subtree
     */
    expr::ptr_type & operator[] (globals::size_type n) {
        return childs[n];
    }

    /** \brief Terminal for move_to method */
    void move_to() {
    }

    /** \brief Make supplied trees subtrees of this node
     *
     *  This function is accepting variable number of parameters.
     *
     *  Using move semantics move supplied trees to become subtrees
     *  of this node in order they were given as parameters. You are not
     *  allowed to use parameters after they was passed using this method
     *
     *  When user supply more arguments then arity of this node it will 
     *  cause accessing to negative index in array. On the other hand
     *  when user supply less arguments then arity of this node they will
     *  be aligned to the end of the childs (zeroth child will not be 
     *  filled).
     */
    template <class T, class ... TList>
    void move_to(T && t, TList && ... plist) {
        childs[childs.size() - sizeof...(plist) - 1] = std::move(t);
        move_to(std::move(plist)...);
    }
};


} // end of namespace ex

#endif
