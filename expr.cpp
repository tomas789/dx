#include "expr.hpp"

tree_type build(const std::string& str) {
    auto *m = new function<mul>;
    m->childs[0] = std::move(expr::ptr_type(new function<constant>(5)));
    m->childs[1] = std::move(expr::ptr_type(new function<variable>(1)));
    //auto *s = new function<struct sin>;
    //s->childs[0] = std::move(expr::ptr_type(m));
    return expr::ptr_type(m);
}

