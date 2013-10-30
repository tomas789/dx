#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <set>
#include <functional>
#include <iomanip>
#include <mutex>
#include <thread>

#include "individual.hpp"

int main(int argc, char ** argv) {
    std::vector<std::string> vars = { "x" };

    individual lhs = make_random(vars, 2);
    individual rhs = make_random(vars, 2);
    std::cout << "LHS : " << lhs << std::endl;
    std::cout << "RHS : " << rhs << std::endl;
    
    individual cross = lhs.crossover(rhs);
    std::cout << "CROSSOVER : " << cross << std::endl;

    return 0;
}

