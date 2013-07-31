#include <iostream>
#include "expr.hpp"

int main(int argc, char ** argv) {

	using namespace ex;

    //tree_type t(new function<constant>(5));
    //auto d = std::move(t->derive(0));

    auto t = std::move(build("4"));
	auto tree = std::move(t->clone());
    //auto deri = std::move(tree->derive(0));
    std::cout << "BUILD:  " << tree->to_string() << std::endl;
    //          << "DERIVE: " << deri->to_string() << std::endl;

    //std::cout << "TREE:   " << t->to_string() << std::endl
    //          << "DERIVE: " << d->to_string() << std::endl;

	return 0;
}
