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
#include <algorithm>
#include <iterator>

#include "individual.hpp"
#include "population.hpp"
#include "random.hpp"
#include "genetics.hpp"

int main(int argc, char ** argv) {
    const std::size_t pop_size = 30;
    std::vector<std::string> vars = { "x" };

    population p;
    p.reserve(10);

    auto gen = std::bind(make_random, std::cref(vars), 3);
    for (std::size_t i = 0; i < pop_size; ++i)
        p.push_back(std::move(gen()));

    std::cout << "Pop size : " << p.size() << std::endl;

    for (auto it = p.cbegin(); it != p.cend(); ++it) {
        std::cout << it - p.cbegin() << " : " << *it << std::endl;
    }

    for (std::size_t i = 0; i < 100; ++i) {
        auto fst = random_between((std::size_t)0, pop_size - 1);
        auto snd = random_between((std::size_t)0, pop_size - 1);
        auto cross = p[fst].crossover(p[snd]);
        p[random_between(0, 1) ? fst : snd] = std::move(cross);
    }
    
    std::cout << "CROSSOVERED" << std::endl;
    for (auto it = p.cbegin(); it != p.cend(); ++it) {
        std::cout << it - p.cbegin() << " : " << *it << std::endl;
    }


    return 0;
}

