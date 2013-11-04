#ifndef _GENETICS_HPP
#define _GENETICS_HPP

#include <functional>
#include <map>
#include <type_traits>

#include "individual.hpp"
#include "population.hpp"

class genetics {
    /** \brief Every population have to have this size */
    std::size_t population_size = 20;

    /** \brief Maximum number of populations for simulation */
    std::size_t max_generations = 50;

    /** \brief Every randomly initialized individual have this depth
     *
     *  Individual will be a tree where every leave is at the same depth
     *  set by this variable
     */
    std::size_t initial_individual_depth = 3;

    /** \brief How many times should be crossover repeated when creating new 
     *         generation
     */
    std::size_t crossover_repeat = 3;

    /** \brief How many individuals are placed to tournament */
    std::size_t tournament_size = 5;

    /** \brief Should be evaluation of individuals cached? 
     *
     *  \todo Preset maximum size of cache
     */
    bool use_cache = false;

    /** \brief Whare are the variables contained in individual */
    std::vector<std::string> vars;

    /** \brief Fitness function
     *
     *  This is measuring quality of every individual
     */
    std::function<double(individual)> fitness;

    /** \brief Population container */
    population c;

    /** \brief Generate random subpopulation of current population with size
     *         about `size`
     *
     *  Individuals are chosen uniformly into the subpopulation.
     */
    population::subpop random_subpopulation(std::size_t size);

    /** \brief Cache container
     *
     *  \todo Consider implementing Splay tree or somethind like this
     */
    std::map<std::size_t, double> cache;

    /** \brief Return quality of given individual measured by fitness function */
    double get_fitness(const individual & i);

    /** \brief Compare two individuals according to theirs quality measured by
     *         fitness function
     */
    bool compare_individuals(const individual & lhs, const individual & rhs);

    /** \brief Initialize current population with random individuals */
    void initialize_population();

    /** \brief Perform tournament and return winner
     *
     *  Subpopulation is chosen using `random_subpopulation` method. Individuals
     *  are measured by `compare_individuals`.
     */
    population::iterator tournament(std::size_t size);

public:
    /** \brief Set user defined fitness functino */
    genetics & set_fitness(std::function<double(individual)> f);

    /** \brief Execute genetic simulation */
    void run();
};

#endif
