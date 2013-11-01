#include "random.hpp"

std::mt19937 & make_random_engine() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

bool random_alternative(double p) {
    std::bernoulli_distribution distrib(p);
    auto& engine = make_random_engine();
    return distrib(engine);
}

