#ifndef _GENETICS_HPP
#define _GENETICS_HPP

#include <random>
#include <mutex>
#include <vector>
#include <thread>

#include "expression.hpp"

class genetics {
public:
    typedef std::pair<double, expression> individual_type;

private:
    static std::random_device rd_;
    static std::mt19937 gen_;

    double target(double x);

    typedef std::pair<std::thread::id, individual_type> foreigner_type;
    std::mutex foreigners_lock_;
    std::vector<foreigner_type> foreigners_;

    std::size_t max_foreigners_ = 20;
    individual_type get_foreigner();

public:
    genetics() = default;

    double least_squares(double from, double to, std::size_t pts, 
                         std::function<double(double)> f);

    expression random(const std::vector<std::string> & vars, std::size_t depth);

    void island();
};

#endif
