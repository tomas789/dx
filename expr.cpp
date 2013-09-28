#include "expr.hpp"

namespace ex {
	
tree_type::tree_type(expr * p) 
  : std::unique_ptr<expr>(p) { }

tree_type::tree_type(tree_type && t)
{ operator= (std::move(t)); }

tree_type::tree_type(const tree_type & t)
{ operator= (std::move(t->clone())); }

tree_type & tree_type::operator= (const tree_type & t) {
	std::unique_ptr<expr>::operator= (std::move(t->clone()));
	return *this;
}

tree_type & tree_type::operator= (tree_type && t) {
	std::unique_ptr<expr>::operator= (std::move(t));
	return *this;
}

expr::size_type tree_type::arity() const
{ return operator-> ()->arity(); }

tree_type & tree_type::operator[] (expr::size_type n)
{ return operator-> ()->operator[] (n); }

} // end namespace ex
