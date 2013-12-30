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

    auto fitness = [] (const individual& e) {
        std::vector<double> vals;
        for (double i = -1; i <= 1; i += 0.1) {
            double opt = i * std::sin(i);
            vals.push_back(std::pow(opt - e.eval([&] (std::string) { return i; }), 2));
        }

        double sum = 0;
        std::for_each(
                vals.begin(), vals.end(),
                [&] (double c) { sum += c; }
             );

        return sum / vals.size();
    };

    genetics g;
    g.set_fitness(fitness);
    g.run();

    return 0;

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

