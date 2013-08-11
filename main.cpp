#include <iostream>
#include "expression.hpp"


int main(int argc, char ** argv) {

	expression e = make_constant(5) + make_constant(6);
	expression e2 = e * make_variable(0);
	std::cout << e2.derive(0) << std::endl;

	return 0;
}
