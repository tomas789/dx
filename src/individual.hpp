#ifndef _INDIVIDUAL_HPP
#define _INDIVIDUAL_HPP

#include <ostream>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include "random.hpp"
#include "expr.hpp"

/** \brief This is thin wrapper around tree representing individual of population.
 *         It is supplying basic functionality and comfortable interface.
 */
class individual {

    /** \brief Actual tree */
    std::unique_ptr<ex::expr> c;

    /** \brief Linearize individual
     *
     *  This will put pointers to all nodes in this tree. Order is unspecified.
     *
     *  \todo DFS order guarantee
     *
     *  \return vector of pointers to unique_ptr representing subtree
     */
    std::vector<std::unique_ptr<ex::expr> *> linearize() const;
public:

    /** \brief Construct "null individual" value
     *
     *  There is no-exception guarantee
     */
    individual() noexcept;

    /** \brief Construct individual around given unique_ptr to tree
     *
     *  \param t unique tree rvalue
     */
    individual(std::unique_ptr<ex::expr> && t);

    /** \brief Copy supplied individual
     *
     *  It will deep-copy original individual
     */
    individual(const individual & i);

    /** \brief Construct individual from another one
     *
     *  You are not allowed to use parameter after it was passed to this
     *  ctor.
     *
     *  There is no-exception guarantee;
     *
     *  \todo Make parameter "null individual" value to prevent
     *        null pointer dereference on accidental use.
     */
    individual(individual && i) noexcept;

    /** \brief Assign deep copy to this instance
     *
     *  \return reference to this instance
     */
    individual & operator= (const individual & i);

    /** \brief Assign another individual to this instance using move semantics
     *
     *  It is guaranteed original individual will not be copied. There is no-exception
     *  guarantee.
     *
     *  \return reference to this instance
     *
     *  \todo Make parameter "null individual" value to prevent
     *        null pointer dereference on accidental use.
     */
    individual & operator= (individual && i) noexcept;

    /** \brief Produce another individual by crossovering this with supplied individual
     *
     *  They will be crossovered by two point subtree swap corossover
     *
     *  Neither this nor parameter individual will be changed.
     *
     *  \returns no individual independent on its parrents
     */
    individual crossover(const individual & i) const;

    /** \brief Produce another individual by mutating this 
     *
     *  This will do subtree to leaf mutation. Leaf will always be constant.
     *
     *  \return New individual independent on this one.
     *
     *  \todo Add ability to generate variable
     */
    individual mutate() const;

    /** \brief Hash of this individual
     *
     *  Probability of two different individuals having same hash is close to
     *  `1 / std::numeris_limits<std::size_t>::max()`.
     */
    std::size_t hash() const;

    /** \brief Evaluate this individual for given valuation
     *
     *  Valuation is function from set of variables to eval_type. It is typically
     *  function `f` such as `f : std::size_t -> double`
     */
    double eval(std::function<double(std::string)> vals) const;

    friend std::ostream & operator<< (std::ostream & out, const individual & i);  
};

/** \brief Print linearized individual to output stream 
 *
 *  \return reference to stream
 */
std::ostream & operator<< (std::ostream & out, const individual & i);


/** \brief Make random individual using supplied variables
 *
 *  \todo Consider moving of this function to header `random.hpp`
 */
individual make_random(
        const std::vector<std::string> & vars, 
        std::size_t depth = 0
    );

#endif
