#include <iostream>
#include "expr.hpp"

int main(int argc, char ** argv) {


    tree_type t(new function<constant>(5));
    auto d = std::move(t->derive(0));

    std::cout << "TREE:   " << t->to_string() << std::endl
              << "DERIVE: " << d->to_string() << std::endl;

	return 0;
}
