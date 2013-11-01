#ifndef _GENETICS_HPP
#define _GENETICS_HPP

#include <functional>

#include "individual.hpp"
#include "population.hpp"

class genetics {
    std::size_t population_size = 50;
    std::size_t max_generations = 50;

    std::size_t initial_individual_depth = 3;

    std::vector<std::string> vars;
    std::function<double(individual)> fitness;

    population c;
    individual::subpop random_subpopulation(std::size_t size);

    bool compare_individuals(const individual & lhs, const individual & rhs) const;
    void initialize_population();

public:
    genetics & set_fitness(std::function<double(individual)> f);

    void run();
};

#endif
