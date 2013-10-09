#ifndef N_ARY_HPP
#define N_ARY_HPP

#include <array>

#include "expr.hpp"
#include "globals.hpp"
#include "traits.hpp"

namespace ex {

/**
 * Child (subtree) container.
 */
template <std::size_t Arity>
class n_ary : public expr {
protected:
    std::array<expr::ptr_type, Arity> childs;
    template <class T, class U> friend class function;
public:
    globals::size_type arity() {
        return Arity;
    }

    expr::ptr_type & operator[] (globals::size_type n) {
        return childs[n];
    }

    void move_to() {
    }

    template <class T, class ... TList>
    void move_to(T && t, TList && ... plist) {
        childs[childs.size() - sizeof...(plist) - 1] = std::move(t);
        move_to(std::move(plist)...);
    }
};


} // end of namespace ex

#endif
