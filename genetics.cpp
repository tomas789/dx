#include "genetics.hpp"

#include <random>
#include <cmath>
#include <mutex>
#include <limits>
#include <thread>
#include <vector>
#include <iomanip>

std::random_device genetics::rd_;
std::mt19937 genetics::gen_;

double genetics::target(double x) {
    return x * sin(x);
}

genetics::individual_type genetics::get_foreigner() {
    std::lock_guard<std::mutex> _(foreigners_lock_);

    auto filter = [] (const foreigner_type & f) {
        return std::this_thread::get_id() != f.first;
    };

    auto ind = find_if(foreigners_.begin(), foreigners_.end(), filter);
    return ind->second;
}

double genetics::least_squares(
        double from,
        double to,
        std::size_t pts,
        std::function<double(double)> f) {
    
    double sum = 0;
    double step = (to - from) / pts;

    for (std::size_t i = 0; i <= pts; ++i) {
        double x = step * i + from;
        double curr = f(x) - target(x);
        if (!isnan(curr)) sum += curr * curr;
        else sum += std::numeric_limits<double>::infinity();
    }

    return sum / pts;
}

expression genetics::random(
        const std::vector<std::string> & vars, 
        std::size_t depth) {

    if (depth) {
        // Inner node
        std::uniform_int_distribution<char> d(0, 3);
        
        switch (d(gen_)) {
            case 0: return random(vars, depth-1) + random(vars, depth-1);
            case 1: return random(vars, depth-1) - random(vars, depth-1);
            case 2: return random(vars, depth-1) * random(vars, depth-1);
            case 3: return random(vars, depth-1) / random(vars, depth-1);
            case 4: return sin(random(vars, depth-1));
            case 5: return cos(random(vars, depth-1));
            case 6: return tan(random(vars, depth-1));
            case 7: return log(random(vars, depth-1));
            case 8: return pow(random(vars, depth-1), random(vars, depth-1));
            default: return make_constant(0);
        }

    } else {
        // Leave
        std::uniform_int_distribution<char> d(0, 1);
        std::normal_distribution<> num(0, 5);
        std::uniform_int_distribution<std::size_t> var(0, vars.size() - 1);

        switch (d(gen_)) {
            case 0: return make_constant(num(gen_));
            case 1: return make_variable(vars[var(gen_)]);
            default: return make_constant(0);
        }
    }

}

void genetics::island() {
    std::vector<std::string> vars = { "x" };

    std::vector<std::pair<double, expression>> pop;
    std::generate_n(std::back_inserter(pop), 8, [&] { 
            return std::make_pair(0, random(vars, 6)); 
    });

    for (auto it = pop.begin(); it != pop.end(); ++it) {
        double squares = least_squares(-8, 8, 300, [&](double x) {
            return it->second.evaluate([&](std::string) { return x; });
        });

        std::cout << std::setw(10) << squares << " - " << it->second << std::endl;
        
    }

    auto cmp = [](const std::pair<double, expression> & lhs, 
                  const std::pair<double, expression> & rhs) {
        return lhs.first < rhs.first;
    };

    for (int g = 0; g < 3000; ++g) {

        // Generation
        pop[pop.size() - 1] = std::make_pair(0, random(vars, 6));
        pop[pop.size() - 2] = std::make_pair(0, random(vars, 6));

        

        // Evaluate
        for (auto it = pop.begin(); it != pop.end(); ++it) {
            it->first = least_squares(-1, 1.1, 100, [&] (double x) {
                    return it->second.evaluate([&](std::string) { return x; });
                });

        }



        // Sort
        std::sort(pop.begin(), pop.end(), cmp);

        if (g % 50 == 0) {
            for (auto it = pop.begin(); it != pop.end(); ++it)
                std::cout << std::setw(10) << it->first << " ";
            std::cout << std::endl;
        }
    }

    std::cout << pop[0].second << std::endl;

}

