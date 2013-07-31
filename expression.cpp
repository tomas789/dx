#include "expression.hpp"

expression::expression()
  : tree() { }

expression::expression(const expression& expr)
  : tree(expr.empty() ? NULL : std::move(expr.tree->clone())) { }

expression::expression(expression&& expr)
  : tree(std::move(expr.tree)) { }

expression::expression(ex::tree_type&& unique)
  : tree(std::move(unique)) { }

expression::expression(double c)
{ *this = make_constant(c); }

expression& expression::operator= (const expression& expr) 
{ return *this = std::move(expression(expr)); }

expression& expression::operator= (expression&& expr)
{ tree = std::move(expr.tree); };

bool expression::empty() const
{ return ! tree.get(); }

expression::string_type expression::to_string() const {
	if (! empty()) return tree->to_string();
	return string_type();
}

expression expression::derive(const valuation_type::size_type& val) const {
	if (! empty()) return expression(std::move(tree->derive(val)));
	return expression();
}

expression expression::operator+ (const expression& expr) {
	return ex::tree_type(
		new ex::function<ex::add>(
			std::move(tree->clone()), 
			std::move(expr.tree->clone())
		)
	);
}

expression expression::operator- (const expression& expr) {
	return ex::tree_type(
		new ex::function<ex::sub>(
			std::move(tree->clone()), 
			std::move(expr.tree->clone())
		)
	);
}

expression expression::operator* (const expression& expr) {
	return ex::tree_type(
		new ex::function<ex::mul>(
			std::move(tree->clone()), 
			std::move(expr.tree->clone())
		)
	);
}

expression expression::operator/ (const expression& expr) {
	return ex::tree_type(
		new ex::function<ex::div>(
			std::move(tree->clone()), 
			std::move(expr.tree->clone())
		)
	);
}

expression expression::operator^ (const expression& expr) {
	return ex::tree_type(
		new ex::function<ex::pow>(
			std::move(tree->clone()), 
			std::move(expr.tree->clone())
		)
	);
}

std::ostream& operator<< (std::ostream& out, const expression& expr) {
	return out << expr.to_string();
}

expression make_constant(double c) {
	return ex::tree_type(new ex::function<ex::constant>(c));
}

expression make_variable(std::size_t v) {
	return ex::tree_type(new ex::function<ex::variable>(v));
}
