#include "population.hpp"

population::subpop::subpop(population & pp)
    : p(pp) { }

population::subpop & population::subpop::include(std::size_t i) {
    s.insert(i);
    return *this;
}

population::subpop & population::subpop::include(const std::initializer_list<std::size_t> & list) {
    for (auto it = list.begin(); it != list.end(); ++it)
        s.insert(*it);

    return *this;
}

population::subpop & population::subpop::exclude(std::size_t i) {
    s.erase(i);
    return *this;
}

population::subpop & population::subpop::exclude(const std::initializer_list<std::size_t> & list) {
    for (auto it = list.begin(); it != list.end(); ++it)
        s.erase(*it);

    return *this;
}

std::size_t population::subpop::size() const {
    return s.size();
}

bool population::subpop::contains(std::size_t i) const {
    return s.find(i) != s.end();
}

population::population() { }

population::population(const population & p)
    : c(p.c) { }

population::population(population && p)
    : c(std::move(p.c)) { }

population::population(const subpop & s) {
    for (auto it = s.s.begin(); it != s.s.end(); ++it) {
        push_back(s.p[*it]);
    }
}

population & population::operator= (const population & p) {
    c = p.c;
    return *this;
}

population & population::operator= (population && p) {
    c = std::move(p.c);
    return *this;
}

population::subpop population::subpopulation() {
    return subpop(*this);
}

std::size_t population::size() const {
    return c.size();
}

population::reference population::operator[] (std::size_t i) {
    return c[i];
}

population::const_reference population::operator[] (std::size_t i) const {
    return c[i];
}

population::iterator population::begin() {
    return c.begin();
}

population::iterator population::end() {
    return c.end();
}

population::const_iterator population::begin() const {
    return c.begin();
}

population::const_iterator population::end() const {
    return c.end();
}

population::const_iterator population::cbegin() const {
    return c.cbegin();
}

population::const_iterator population::cend() const {
    return c.cend();
}

void population::reserve(std::size_t n) {
    c.reserve(n);
}

void population::push_back(const value_type & value) {
    c.push_back(value);
}

void population::push_back(value_type && value) {
    c.push_back(std::move(value));
}
