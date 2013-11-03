#include "genetics.hpp"

#include <iostream>
#include <iomanip>
#include <functional>

population::subpop genetics::random_subpopulation(std::size_t size) {
    double r = (double) size / c.size();
    auto subpop = c.subpopulation();
    
    for (std::size_t i = 0; i < c.size(); ++i)
        if (random_alternative(r))
            subpop.include(i);

    return subpop;
}

double genetics::get_fitness(const individual & i) {
    std::size_t h = i.hash();
    auto it = cache.find(h);
    if (cache.end() != it)
        return it->second;
    else {
        double val = fitness(i);
        cache[h] = val;
        return val;
    }
}

bool genetics::compare_individuals(const individual & lhs, const individual & rhs) {
    return get_fitness(lhs) < get_fitness(rhs);
}

void genetics::initialize_population() {
    auto gen = std::bind(make_random, std::cref(vars), initial_individual_depth);
    for (std::size_t i = 0; i < population_size; ++i) {
        c.push_back(std::move(gen()));
    }
}

genetics & genetics::set_fitness(std::function<double(individual)> f) {
    fitness = f;
    return *this;
}

void genetics::run() {
    initialize_population();

    auto cmp = std::bind(
            &genetics::compare_individuals, this,
            std::placeholders::_1, 
            std::placeholders::_2);

    std::sort(c.begin(), c.end(), cmp);

    for (auto it = c.begin(); it != c.end(); ++it) {
        std::cout << std::setw(11) << get_fitness(*it) << " " << *it << std::endl;
    }
}
