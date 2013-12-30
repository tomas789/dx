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
    if (!use_cache) 
        return fitness(i);

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

population::iterator genetics::tournament(std::size_t size) {
    auto subpop = random_subpopulation(size);
    population p(subpop);
    auto max = std::max_element(
            subpop.begin(), subpop.end(),
            std::bind(
                &genetics::compare_individuals, this,
                *std::placeholders::_1,
                *std::placeholders::_2)
        );

    return max;
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

    for (std::size_t i = 0; i < max_generations; ++i) {
        std::sort(c.begin(), c.end(), cmp);

        double sum = 0;
        std::for_each(
                c.begin(), c.end(),
                [&] (const individual & i) { sum += get_fitness(i); }
            );

        std::size_t width = 5;
        std::cout << "G" << std::setw(5) << i
                  << " [0] : " << std::setw(width) << get_fitness(c[0])
                  << " [1] : " << std::setw(width) << get_fitness(c[1])
                  << " [2] : " << std::setw(width) << get_fitness(c[2])
                  << " AVG : " << sum / c.size() << std::endl;

        population p2;

        for (std::size_t j = 0; j < crossover_repeat; ++j) {
            std::size_t fst = random_between((std::size_t)0, c.size() - 1);
            std::size_t snd = random_between((std::size_t)0, c.size() - 1);

            auto indiv = c[fst].crossover(c[snd]);
            p2.push_back(std::move(indiv));
        }

        while (p2.size() != c.size()) {
            std::size_t k = random_between((std::size_t)0, c.size() - 1);
            p2.push_back(c[k]);
        }

        c = std::move(p2);
    }

    return;

    for (auto it = c.begin(); it != c.end(); ++it) {
        std::cout << std::setw(11) << get_fitness(*it) << " " << *it << std::endl;
    }
}
