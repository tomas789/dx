#ifndef _INDIVIDUAL_HPP
#define _INDIVIDUAL_HPP

#include <ostream>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include "random.hpp"
#include "expr.hpp"

class individual {
    std::unique_ptr<ex::expr> c;

    std::vector<std::unique_ptr<ex::expr> *> linearize() const;
public:
    individual() noexcept;
    individual(std::unique_ptr<ex::expr> && t);
    individual(const individual & i);
    individual(individual && i) noexcept;

    individual & operator= (const individual & i);
    individual & operator= (individual && i) noexcept;

    individual crossover(const individual & i) const;
    individual mutate() const;

    friend std::ostream & operator<< (std::ostream & out, const individual & i);  
};

std::ostream & operator<< (std::ostream & out, const individual & i);

individual make_random(
        const std::vector<std::string> & vars, 
        std::size_t depth = 0);

#endif
