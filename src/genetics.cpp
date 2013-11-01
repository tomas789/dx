#include "genetics.hpp"

individual::subpop random_subpopulation(std::size_t size) {
    double p = (double) size / p.size();
    auto subpop = p.subpopulation();
    
    for (std::size_t i = 0; i < p.size(); ++i)
        if (random_alternative(p))
            subpop.include(i);

    return subpop;
}

bool genetics::compare_individuals(const individual & lhs, const individual & rhs) const {
    return fitness(lhs) < fitness(rhs);
}

void genetics::initialize_population() {
    auto gen = std::bind(make_random, std::cref(vars), initial_individual_depth);
    for (std::size_t i = 0; i < population_size; ++i) {
        p.push_back(std::move(gen()))'
    }
}

genetics & genetics::set_fitness(std::function<double(individual)> f) {
    fitness = f;
}

void genetics::run() {
    initialize_population();
}
