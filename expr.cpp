#include "expr.hpp"

namespace ex {
    
tree_type::tree_type(expr * p) : e_(p) { 
}

tree_type::tree_type(tree_type && t) { 
    *this = std::move(t); 
}

tree_type::tree_type(const tree_type & t) {
    *this = t;
}

tree_type & tree_type::operator= (const tree_type & t) {
    e_ = std::move(t.e_->clone().e_);
    return *this;
}

tree_type & tree_type::operator= (tree_type && t) {
    e_ = (std::move(t.e_));
    return *this;
}

bool tree_type::empty() const {
    return !e_.get();
}

expr::size_type tree_type::arity() const { 
    return e_->arity();
}

tree_type & tree_type::operator[] (expr::size_type n) { 
    return e_->operator[] (n);
}

std::unique_ptr<expr> & tree_type::operator-> () {
    return e_;
}

const std::unique_ptr<expr> & tree_type::operator-> () const {
    return e_;
}

} // end namespace ex
