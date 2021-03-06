#include "individual.hpp"

#include <iostream>
#include <stack>
#include <vector>
#include <string>

#include "expr.hpp"
#include "function.hpp"
#include "function_base.hpp"
#include "visitor.hpp"
#include "visitor_impl.hpp"

std::unique_ptr<ex::expr>  make_random_impl(
        const std::vector<std::string> & vars,
        std::size_t depth);

std::vector<std::unique_ptr<ex::expr> *> individual::linearize() const {
    std::vector<std::unique_ptr<ex::expr> * > v;
    std::stack<std::unique_ptr<ex::expr> * > queue;
    queue.push(const_cast<std::unique_ptr<ex::expr> *>(&c));

    while (! queue.empty()) {
        std::unique_ptr<ex::expr> * n = queue.top();
        queue.pop();

        v.push_back(n);

        for (std::size_t i = 0; i < (*n)->arity(); ++i) {
            auto & v = (*n)->operator[] (i);
            queue.push(&v);
        }
    }

    return v;
}

individual::individual() noexcept { }

individual::individual(std::unique_ptr<ex::expr> && t)
    : c(std::move(t)) { }


individual::individual(const individual & i) {
    *this = i;
}

individual::individual(individual && i) noexcept {
    *this = std::move(i);
}

individual & individual::operator= (const individual & i) {
    c = std::move(i.c->clone());
    return *this;
}

individual & individual::operator= (individual && i) noexcept {
    c = std::move(i.c);
    return *this;
}

individual individual::crossover(const individual & i) const {
    individual this_copy(*this);

    std::unique_ptr<ex::expr> * swap_lhs;
    std::unique_ptr<ex::expr> * swap_rhs;

    {
        std::vector<std::unique_ptr<ex::expr> * > lin_lhs = this_copy.linearize();
        swap_lhs = lin_lhs[random_between((std::size_t)0, lin_lhs.size() - 1)];
    }

    {
        std::vector<std::unique_ptr<ex::expr> * > lin_rhs = i.linearize();
        swap_rhs = lin_rhs[random_between((std::size_t)0, lin_rhs.size() - 1)];
    }
    
    std::unique_ptr<ex::expr> replacement = (*swap_rhs)->clone();
    std::swap(*swap_lhs, replacement);

    return this_copy;
}

individual individual::mutate() const {
    individual new_individual(*this);
    std::unique_ptr<ex::expr> * subtree;
    
    {
        std::vector<std::unique_ptr<ex::expr> * > lin_rhs = new_individual.linearize();
        subtree = lin_rhs[random_between((std::size_t)0, lin_rhs.size() - 1)];
    }

    subtree->reset(new ex::function<ex::base::constant>(random_normal(0.0, 1.0)));

    return new_individual;
}

std::size_t individual::hash() const {
    ex::generic_visitor<ex::hash_visitor> v;
    c->accept(v);
    return v->get_result();
}

double individual::eval(std::function<double(std::size_t)> vals) const {
    ex::eval_visitor v_impl(vals);
    ex::generic_visitor<ex::eval_visitor> v(v_impl);
    c->accept(v);
    return v->get_result();
}

std::ostream & operator<< (std::ostream & out, const individual & i) {
    ex::generic_visitor<ex::latex_visitor> v;
    i.c->accept(v);
    return out << v->get_result();
}

std::unique_ptr<ex::expr> make_inner(
        const std::vector<std::string> & vars,
        std::size_t depth) {
    auto rec = [&] { return make_random_impl(vars, depth - 1); };

    switch (random_between(0, 5)) {
        case 0:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::add>(rec(), rec()));
        case 1:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::sub>(rec(), rec()));
        case 2:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::mul>(rec(), rec()));
        case 3:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::div>(rec(), rec()));
        case 4:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::sin>(rec()));
        case 5:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::cos>(rec()));
    }

    return nullptr;
}

std::unique_ptr<ex::expr> make_leave(const std::vector<std::string> & vars) {
    static const double mean = 0;
    static const double stddev = 1;

    switch (random_between(0, vars.size() == 0 ? 0 : 1)) {
        case 0:
            /* return constant */
            return std::unique_ptr<ex::expr>(
                    new ex::function<ex::base::constant>(random_normal(mean, stddev)));
        case 1:
            /* return variable */
            std::size_t var = random_between((std::size_t)0, vars.size() - 1);
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::variable>(var));
    }

    return nullptr;
}

std::unique_ptr<ex::expr> make_random_impl(
        const std::vector<std::string> & vars,
        std::size_t depth) {
    if (depth > 0) return std::move(make_inner(vars, depth));
    else return std::move(make_leave(vars));
}

individual make_random(
        const std::vector<std::string> & vars,
        std::size_t depth) {
    return individual(make_random_impl(vars, depth));
}

