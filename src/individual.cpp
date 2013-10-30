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

    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << it - v.begin() << " : ";
        ex::generic_visitor<ex::printer_visitor> v;
        (**it)->accept(v);
        std::cout << v->get_result() << std::endl;
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
    std::vector<std::unique_ptr<ex::expr> * > lin_lhs = linearize();
    std::vector<std::unique_ptr<ex::expr> * > lin_rhs = linearize();
}

individual individual::mutate() const {

}

std::ostream & operator<< (std::ostream & out, const individual & i) {
    ex::generic_visitor<ex::printer_visitor> v;
    i.c->accept(v);
    return out << v->get_result();
}

std::unique_ptr<ex::expr>  make_random_impl(
        const std::vector<std::string> & vars,
        std::size_t depth);

std::unique_ptr<ex::expr> make_inner(
        const std::vector<std::string> & vars,
        std::size_t depth) {
    auto rec = [&] { return make_random_impl(vars, depth - 1); };

    switch (random_between(0, 1)) {
        case 0:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::add>(rec(), rec()));
        case 1:
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::sub>(rec(), rec()));
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
            std::string var = vars[random_between((std::size_t)0, vars.size() - 1)];
            return std::unique_ptr<ex::expr>(new ex::function<ex::base::variable>(var));
    }

    return nullptr;
}

std::unique_ptr<ex::expr>  make_random_impl(
        const std::vector<std::string> & vars,
        std::size_t depth) {
    if (depth > 0) return std::move(make_inner(vars, depth - 1));
    else return std::move(make_leave(vars));
}

individual make_random(
        const std::vector<std::string> & vars,
        std::size_t depth) {
    return individual(make_random_impl(vars, depth));
}

