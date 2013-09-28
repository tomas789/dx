#include <iostream>
#include <string>
#include <vector>

#include "expression.hpp"
#include "exparser.hpp"

int main(int argc, char ** argv) {
    (std::cout << "Type expression : ").flush();
    std::string r;
    std::getline(std::cin, r);

    exparser p;
    try {
        expression e = p.parse(r);
        std::vector<double> val = { 1, 2 };

        std::cout << "INPUT : " << e << std::endl
                  << "DERIVED BY x_0 : " << e.derive(0) << std::endl
                  << "EVALUATED FOR x_0 = 1, x_1 = 2 : " << e.evaluate(val) << std::endl
                  << "EVALUATED DERIVATION : " << e.derive(0).evaluate(val) << std::endl;
    } catch (std::string e) {
        std::cerr << "EXCEPTION: " << e << std::endl;
    }

	return 0;
}
