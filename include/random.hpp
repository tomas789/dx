#ifndef _RANDOM_HPP
#define _RANDOM_HPP

#include <random>
#include <type_traits>

/** \brief This should be only instance of random engine generator in
 *         whole program.
 *
 *  It is here to guarantee it will not repeat random number sequence
 *  twice.
 *
 *  \return reference to random number generator
 */
std::mt19937 & make_random_engine();

/** \brief Generate random number from interval
 *
 *  Both parameters have to be of the same type explicitly (no implicit
 *  conversion will be performed).
 *
 *  Random number will be generated from interval [begin, end] when 
 *  arguments are of integral type and from [begin, end) wher they are
 *  floating point.
 *
 *  \todo Do basic type conversion when begin and end are not the same
 *        type.
 *
 *  \return random number in interval as defined above of same type as 
 *          arguments were.
 */
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

/** \brief Generate random number from normal distribution
 *
 *  Both parameters have to be of the same type explicitly (no implicit
 *  conversion will be performed). They have to bo floating point numbers.
 *
 *  \todo Do basic type conversion when begin and end are not the same
 *        type.
 *
 *  \param mean Mean of normal distribution
 *  \param stddev Standard deviation of normal distribution
 *
 *  \return random number from normal distribution of the same type as
 *          arguments were.
 */
template <typename T>
T random_normal(T mean, T stddev) {
    std::normal_distribution<T> distrib(mean, stddev);
    auto& engine = make_random_engine();
    return distrib(engine);
}

bool random_alternative(double p);

#endif
