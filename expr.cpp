#include "expr.hpp"

ex::tree_type ex::build(const std::string& str) {
	auto c = new function<constant>(5);
	auto v = new function<variable>(0);
	auto m = new function<struct mul>(tree_type(c), tree_type(v));
	return tree_type(m);
}

