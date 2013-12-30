#ifndef _POPULATION_HPP
#define _POPULATION_HPP

#include <vector>
#include <set>

#include "random.hpp"
#include "individual.hpp"

class population {
public:
    typedef individual value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef std::vector<value_type> container;
    typedef typename container::iterator iterator;
    typedef typename container::const_iterator const_iterator;

    /** \brief Iterator-like class representing subset of given
     *         population (subpopulation)
     *
     *  This is not holding actual values or references to original individuals.
     *  Instead it holds reference to population and set of identifiers of 
     *  selected individuals (its numbers).
     */
    struct subpop {
        subpop(population & p);

        subpop & include(std::size_t i);
        subpop & include(const std::initializer_list<std::size_t> & list);
        subpop & exclude(std::size_t i);
        subpop & exclude(const std::initializer_list<std::size_t> & list);

        std::size_t size() const;
        bool contains(std::size_t) const;

        friend class population;

    private:
        population & p;
        std::set<std::size_t> s;
    };

protected:
    /** \brief Population container. */
    container c;

public:
    /** \brief Empty population */
    population();

    /** \brief Initialize population by different instance of this class
     *
     *  Every individual is copyed using its copy-constructor
     */
    population(const population & p);

    /** \brief Inidialize population by different instance of this class
     *
     * Every individual is moved using its move-constructor
     */
    population(population && p);

    /** \brief Initialize population by subpopulation of different population
     *
     *  Every individual is copyed using its copy-constructor.
     */
    population(const subpop & s);

    population & operator= (const population & p);
    population & operator= (population && p);

    /** \brief Get subpopulation iterator starting with empty subset */
    subpop subpopulation();

    /** \brief Returns the number of individuals in the population */
    std::size_t size() const;

    /** \brief Access specified element
     *
     *  Returns a reference to the element at specified location pos. No bounds
     *  checking is performed.
     */
    reference operator[] (std::size_t i);
    
    /** \brief Access specified element
     *
     *  Returns a constant reference to the element at specified location pos. 
     *  No bounds checking is performed.
     */
    const_reference operator[] (std::size_t i) const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    /** \brief Reserve space for `n` individuals */
    void reserve(std::size_t n);

    /** \brief Add individual to population
     *
     *  Copy-constructor of supplied individual will be used
     */
    void push_back(const value_type & value);

    /** \brief Add individual to population
     *
     *  Move-constructor of supplied individual will be used
     */
    void push_back(value_type && value);
};

#endif
