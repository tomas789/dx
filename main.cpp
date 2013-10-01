#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "expression.hpp"

std::map<std::string, double> val_cache;

double eval(const std::string & var_name);

int main(int argc, char ** argv) {
    (std::cout << "Type expression : ").flush();
    std::string r;
    std::getline(std::cin, r);

    try {
        expression e = parse(r);
        auto c = e.evaluate(eval);

        std::cout << "INPUT : " << e << std::endl
                  << "DERIVED BY x : " << e.derive("x") << std::endl
                  << "EVALUATED : " << c << std::endl
                  << "EVALUATED DERIVATION BY x : " << e.derive("x").evaluate(eval) << std::endl;
    } catch (std::string e) {
        std::cerr << "EXCEPTION: " << e << std::endl;
    }

    return 0;
}

double eval(const std::string & var_name) {
    auto it = val_cache.find(var_name);
    if (it != val_cache.end()) return it->second;
    
    std::cout << " -=- ENTER VALUE FOR " << var_name << " : ";
    std::cout.flush();

    double c;
    std::cin >> c;
    
    return val_cache[var_name] = c;
}
