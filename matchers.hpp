#ifndef _MATCHERS_HPP
#define _MATCHERS_HPP

#include "function_base.hpp"
#include "function.hpp"

namespace ex {

struct matcher {

    virtual bool match(expr * e);

};

namespace match {

template <typename T>
struct type : public matcher {
    virtual bool match(expr * e) {
        if (e == nullptr) return false;
        return typeid(*e) == typeid(T);
    }
};

struct constant_near : public matcher {
private:
    double c_;
    double eps_;
public:
    constant_near(double c, double eps) : c_(c), eps_(eps) { }
    virtual bool match(expr * e) {
        if (auto n = dynamic_cast<function<base::constant>*>(e))
            return std::abs(n->value - c_) < eps_;
        return false;
    }
};

}

}

#endif
