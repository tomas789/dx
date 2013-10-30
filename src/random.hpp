#ifndef _RANDOM_HPP
#define _RANDOM_HPP

#include <random>
#include <type_traits>

std::mt19937 & make_random_engine();

template <
        typename T,
        typename = typename std::enable_if<
                std::is_integral<T>::value || 
                std::is_floating_point<T>::value
            >::type
    >
T random_between(T begin, T end) {
    typename std::conditional<
            std::is_integral<T>::value,
            std::uniform_int_distribution<T>,
            std::uniform_real_distribution<T>
        >::type distrib(begin, end);

    auto& engine = make_random_engine();
    
    return distrib(engine);
}

template <typename T>
T random_normal(T mean, T stddev) {
    std::normal_distribution<T> distrib(mean, stddev);
    auto& engine = make_random_engine();
    return distrib(engine);
}

#endif
